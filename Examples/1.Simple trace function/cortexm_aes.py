# aes128 from https://github.com/Ko-/aes-armcortexm

import numpy as np
from binascii import unhexlify, hexlify
from rainbow.generics import rainbow_arm
from rainbow.utils import hw, plot
import matplotlib.pyplot as plt
import lascar
from lascar.tools.aes import sbox


def aes_encrypt(key, plaintext):
    e = rainbow_arm(sca_mode=True)
    e.load("aes.bin", typ=".elf")

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
    # ciphertext = e[buf_out:buf_out+16]

    # Hamming weight + noise to pretend we're on a real target
    trace = np.array([hw(i) for i in e.sca_values_trace]) + np.random.normal(
        0, 1.0, (len(e.sca_values_trace))
    )
    return trace

class CortexMAesContainer(lascar.AbstractContainer):

    def generate_trace(self,idx):
        plaintext = np.random.randint(0,256,(16,),np.uint8)
        leakage = np.array(aes_encrypt(KEY, plaintext.tobytes())) 
        return lascar.Trace(leakage, plaintext)


N = 1000
KEY = bytes( bytearray.fromhex("cafebabedeadbeef0001020304050607"))

container = CortexMAesContainer(N)

plot(container[:5].leakages)
plt.plot(np.mean(container[:5].leakages,  axis=0))

plt.show()

cpa_engines = [lascar.CpaEngine(f'cpa{i}',lambda plaintext, key_byte, index=i: sbox[plaintext[index]^key_byte], range(256)) for i in range(16)]
s = lascar.Session(CortexMAesContainer(N), engines=cpa_engines, name="lascar CPA").run()

key = bytes([  engine.finalize().max(1).argmax() for engine in cpa_engines])
print("Key is :", hexlify(key).upper())

plot(cpa_engines[1].finalize(), highlight=KEY[1])
