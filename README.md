# WebVRF

This library lets you use Verifiable Random Functions (VRF) in your web browser. It is based on the [libsodium](https://libsodium.gitbook.io/doc/) cryptographic library and provides a simple interface for generating VRF proofs and verifying them.

## ⚠️ SECURITY WARNING ⚠️

**IMPORTANT:** This VRF (Verifiable Random Function) library has not been officially audited by security professionals. As such, it should be considered experimental and potentially unreliable.
Use this library at your own risk. It is not recommended for use in production environments, especially those handling sensitive data or requiring high security assurances. For critical applications, please seek a professionally audited and well-established cryptographic library.

## Function Overview

### VRF.init()

Initializes the VRF class. This must be called before accessing other functions.

- **Returns**: Promise<void>
- **Async**: Yes
- **Arguments**: None
- **Description**: Sets up the WebAssembly module and prepares the VRF functions for use.

### VRF.keypair()

Generates a new key pair for VRF usage.

- **Returns**:
  ```typescript
  {
    publicKey: Uint8Array,
    secretKey: Uint8Array,
    result: number
  }
  ```
- **Async**: No
- **Arguments**: None
- **Description**: Creates a new key pair specifically for VRF. It's recommended to use this function rather than other key generation methods for VRF operations.

### VRF.prove(sk, message)

Generates a VRF proof based on a secret key and a message.

- **Returns**:
  ```typescript
  {
    proof: Uint8Array,
    result: number
  }
  ```
- **Async**: No
- **Arguments**:
  - `sk` (Uint8Array): Secret VRF key
  - `message` (Uint8Array): VRF proof seed
- **Description**: Produces a VRF proof using the provided secret key and message (seed).

### VRF.verify(pk, proof, message)

Verifies a VRF proof and returns its corresponding output.

- **Returns**:
  ```typescript
  {
    output: Uint8Array,
    result: number
  }
  ```
- **Async**: No
- **Arguments**:
  - `pk` (Uint8Array): Public VRF key
  - `proof` (Uint8Array): VRF proof
  - `message` (Uint8Array): Seed used for VRF proof generation
- **Description**: Checks the validity of a VRF proof and returns the verified output if successful.

### VRF.proofToHash(proof)

Converts a VRF proof to its corresponding hash without verification.

- **Returns**:
  ```typescript
  {
    hash: Uint8Array,
    result: number
  }
  ```
- **Async**: No
- **Arguments**:
  - `proof` (Uint8Array): VRF proof
- **Description**: Generates the hash of a given proof without performing verification.

## Notes on Return Values

- All functions (except `init()`) return an object that includes a `result` property. This `result` is typically a number where:
  - `0` indicates success
  - Non-zero values indicate various types of failures (refer to the libsodium documentation for specific error codes)
- It's recommended to always check the `result` value to ensure the operation was successful before using the returned data.

## Usage Example

```javascript
async function runVRF() {
  await VRF.init();

  const { publicKey, secretKey, result: keyGenResult } = VRF.keypair();
  if (keyGenResult !== 0) {
    console.error("Key generation failed");
    return;
  }

  const message = new TextEncoder().encode("Hello, VRF!");
  const { proof, result: proofResult } = VRF.prove(secretKey, message);
  if (proofResult !== 0) {
    console.error("Proof generation failed");
    return;
  }

  const { output, result: verifyResult } = VRF.verify(
    publicKey,
    proof,
    message
  );
  if (verifyResult !== 0) {
    console.error("Proof verification failed");
    return;
  }

  console.log("VRF Output:", output);
}

runVRF().catch(console.error);
```

This library provides a robust implementation of VRF functions, suitable for various cryptographic applications requiring verifiable randomness.

## Recompiling from source

Below instructions worked on Ubuntu 24.04 LTS.

### 1. Clone this repository & enter clone directory

### 2. Install Emscripten

This software allows translation of C code to WebAssembly. Installation instructions are below and were copier from [emscripten.org](https://emscripten.org/docs/getting_started/downloads.html).

```
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh
cd ..
```

### 3. Fetch @appliedblockchain/sodium-native from NPM

This is a fork of [sodium native](https://github.com/sodium-friends/sodium-native) made by Applied Blockchain that adds necessary VRF functions

```bash
npm pack @appliedblockchain/sodium-native
tar -xvf appliedblockchain-sodium-native-3.3.2.tgz
cd package
```

### 4. Install dependencies and fetch Algorand libsodium

This install necessary NPM dependencies & fetches Algorand libsodium from [Applied Blockchain libsodium fork](https://github.com/appliedblockchain/libsodium/)\
**May take a long time**

```bash
yarn && yarn fetch-libsodium
```

### 5. Make Algorand libsodium

This makes libsodium from source\
**May take a long time**

```bash
cd deps/libsodium-1.0.16-algorand/
emconfigure ./configure --disable-shared --prefix="$PWD/libsodium-wasm"
emmake make
emmake make install
cd ../../
```

### 6. Copy vrf_wrapper.c to package

```bash
cp ../vrf_wrapper.c .
```

### 7. Compile vrf_wrapper using Emscripten

```bash
emcc -O3 vrf_wrapper.c -I./deps/libsodium-1.0.16-algorand/src/libsodium/include -L./deps/libsodium-1.0.16-algorand/src/libsodium/.libs -lsodium -s WASM=1 -s EXPORTED_FUNCTIONS="['_malloc', '_free', '_vrf_publickeybytes', '_vrf_secretkeybytes', '_vrf_proofbytes', '_vrf_outputbytes']" -s EXPORTED_RUNTIME_METHODS="['ccall', 'cwrap']" -s ENVIRONMENT='web' -s INCOMING_MODULE_JS_API="['onRuntimeInitialized']" -sSINGLE_FILE -o vrf.js
```

This should create _vrf.js_ and _vrf.wasm_ output files in package directory.

### 8. Export vrf.js

You need to replace `const vrf_js_base64 = "..."` in _index.js_ with the base64-encoded content of _vrf.js_.

You can generate it with this command:

```bash
base64 vrf.js | tr -d '\n'
```

Congrats, now you can use VRF in your web browser. To check, you can launch the _index.html_ file.

```
python3 -m http.server
```

Then click on your localhost link (most likely 0.0.0.0:8000).
You can inspect the correctness of the library using the developer console (F12).

It should look something like this:

```
(index):41 Public Key: Uint8Array(32)
(index):42 Secret Key: Uint8Array(64)
(index):46 Proof: Uint8Array(80)
(index):49 Verification result: Success
(index):50 Output: Uint8Array(64)
(index):53 Hash: Uint8Array(64)
```
