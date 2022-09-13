from binascii import hexlify

import lascar
import numpy as np
from lascar.tools.aes import sbox
from rainbow.generics import rainbow_arm
from rainbow.utils import hw
from visplot import plot
from tqdm import tqdm
import trsfile as trs
from trsfile import trs_open, Trace, SampleCoding, TracePadding, Header
from trsfile.parametermap import TraceParameterMap, TraceParameterDefinitionMap
from trsfile.traceparameter import ByteArrayParameter, ParameterType, TraceParameterDefinition
from Crypto.Cipher import AES
from numba import jit
import matplotlib.pyplot as plt
import  argparse


def check_aes_ecb_enc(plaintext, key):
    if (len(plaintext) == 16 and len(key) == 16):
        cipher = AES.new(key, AES.MODE_ECB)
        ciphertext = cipher.encrypt(bytes(plaintext))
        return bytearray(ciphertext)

@jit(parallel=True)
def aes_encrypt(e ,key, plaintext):

    e.mem_trace = True
    e.trace_regs = True

    key_addr = 0xDEAD0000
    e[key_addr] = key
    rk_addr = 0xDEAD1000
    e[rk_addr] = key

    # AES_128_keyschedule(key, rk+16)
    e["r0"] = key_addr
    e["r1"] = rk_addr + 16
    e.start(e.functions["AES_128_keyschedule"] | 1, 0)

    buf_in = 0xDEAD2000
    buf_out = 0xDEAD3000
    e[buf_in] = plaintext
    e[buf_out] = b"\x00" * 16  # Need to do this so this buffer is mapped into unicorn

    # AES_128_encrypt(rk, buf_in, buf_out)
    e["r0"] = rk_addr
    e["r1"] = buf_in
    e["r2"] = buf_out
    # e.trace_reset()
    e.start(e.functions["AES_128_encrypt"] | 1, 0)

    ## Read out the ciphertext
    ciphertext = e[buf_out:buf_out+16]

    # Hamming weight + noise to pretend we're on a real target
    trace = np.array([hw(i) for i in e.sca_values_trace]) + np.random.normal(
        0, 1.0, (len(e.sca_values_trace)))
    return trace,ciphertext

def cw_in_out_convert_trs_legacy(text_in, text_out):
    dest = bytearray()
    dest.extend(text_in)
    dest.extend(text_out)
    return dest

if __name__ == "__main__":

        from random import getrandbits

        import numpy as np
        from rainbow.utils import hw

        # from visplot import plot

        e = rainbow_arm(sca_mode=True)
        e.load("aes.bin", typ=".elf")

        traces = []
        ciphertexts = []
        plaintexts = []
        N = 5000
        KEY = bytes(bytearray.fromhex("cafebabedeadbeef0001020304050607"))
        with trs_open(
                "test.trs",  # File name of the trace set
                'w',  # Mode: r, w, x, a (default to x)
                # Zero or more options can be passed (supported options depend on the storage engine)
                engine='TrsEngine',  # Optional: how the trace set is stored (defaults to TrsEngine)
                headers={  # Optional: headers (see Header class)
                    Header.TRS_VERSION: 2,
                    Header.SCALE_X: 1e-6,
                    Header.SCALE_Y: 0.1,
                    Header.DESCRIPTION: 'Exported  from CW ',
                },
                padding_mode=TracePadding.AUTO,  # Optional: padding mode (defaults to TracePadding.AUTO)
                live_update=True  # Optional: updates the TRS file for live preview (small performance hit)
        ) as traces_trs:
         for i in tqdm(range(N),desc='Capturing and save traces'):
                 plaintext = bytes([getrandbits(8) for i in range(16)])
                 trace, ciphertext = aes_encrypt(e,KEY,plaintext)
                 ciphertext_py = check_aes_ecb_enc(plaintext,KEY)
                 if ciphertext_py !=  ciphertext:
                    print("Are you have some problems!!")
                 print("Plaintext is :", hexlify(plaintext).upper())
                 print("Ciphertext is :", hexlify(ciphertext).upper())
                 traces_trs.append(Trace(SampleCoding.FLOAT, trace, TraceParameterMap(
                {'LEGACY_DATA': ByteArrayParameter(
                    cw_in_out_convert_trs_legacy(plaintext, ciphertext))})))


        traces_trs.close
        print("Convertion trace completed")
        #traces = np.array(traces)
        #plt.plot(traces[0])
        #plt.show()
        print(len(traces))
        #traces += np.random.normal(0, 1, size=traces.shape)

        #v = plot(traces, dontrun=True)
        #v.multiple_select(0)
        #v.run()