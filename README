# Word Prediction

This a project mainly to test Web Assembly.

# How to test it?

To test the project, you will need:
- Docker (to compile WASM file)
- make

Optional:
- node (to generate the mydb file)
- http-server (to start a web server quickly ```npm install -g http-server```)

# Binary

To test without Web Assembly, you can simplify do ```make re && ./build/my_autocomplete```, you should get:
```
[...]
he
a
==== Testing ====
Does "Hello" has been found ? Yes
Does "Hell" has been found ? No
Does "Mom" has been found ? No
Predict word for "Hel":
=> Predict "lo" with a weight of 20000000
====> Found hello
Predict word for "Lok":
=> Predict "'tar" with a weight of 20000000
====> Found lok'tar
Predict word for "Nothing":
=> Unable to predict
Predict word for "examine":
=> Unable to predict
```

# Web Assembly
To test with WebAssembly (Docker must be up and running):
```
$> make wasm
$> http-server .
```