```text
       .:
   .  .++-                                                                .:---
 :++: =++                                                             ..:------
 :++:.++-                                                          .::::------=.
 :++-.++-                                                        .:::::------=+
  +++.++=                                                     .:::::::-----:-++
  .++==+=:::::.                                            .::::::::------: :++
   .+=-:::------:--------::..                            .:::::::::------.  :++
  -+**+=:::-------============--:.                     ::::::::::------:    .++
 :******-::-------==================-:.             .:::::::::::------.     .++
 :+****=:::-------=====================-:.        .:::::::::::------:        ++
.::---:::::-------============================--:::::::::::::------.         ++
::::::::::-------============================--::::::::::::--------::::....  ++
::::::::--------===========================--::::::::::::-------=============+++====------::::::::
:::::::-------============================-:::::::::::::-------==============+++===================-
.::::------. :======+++=================--::::::::::::-------================+++====================
 .:------.    :===+++++===============--::::::::::::-------===================++===================
   ..:..       .++++=+++=============--::::--:::::-------=====================++=================-
              :++=====+++===========-::-==+++=::-------=======================++================:
            .=++-  .-==++=============++++=+++-------=========================++==============-.
           =++=.      .=++=======+++++=--::-+++----===========================+++===========-.
         -++=.          =+-.=++++=--:.::----=++=---===========================+++=========:.
       .=++:                .::.        ..:--+++-:::----------================+++=======---:
     :=++-                                  .=++-::::::::::::::---------------+++----:-------
.:-=+++=.                                    .+++.::::::::::::::::::::::::::::+++-:::::------.
=+=-:.                                        -++.    .....:::::::::::::::::::+++-::-------:.
                                               ++=             ........:::::--=++-:::::..
                                               -++.                           -++.
                                                ++=                           -++:
                                                -++-:                         :++:
                                                 :-+++=-.                     :++-
                                                    .-=+++                    .+++=-:.
                                                        ..                     .:-=+++++-
                                                                                     .::.    
```

# Kuznyechik (GOST R 34.12-2015) C++ Implementation

This project provides a C++ implementation of the Russian block cipher **Kuznyechik**, also known as "Grasshopper", as specified in the GOST R 34.12-2015 standard. It was developed as part of a seminar paper for the *Data Structures and Algorithms* course at the Faculty of Organization and Informatics, University of Zagreb.

## 🔐 About the Algorithm
Kuznyechik is a modern symmetric block cipher that features:
- 128-bit data blocks
- 256-bit keys
- 10 rounds of SPN (Substitution-Permutation Network)
- S-box substitution, R/L-transformations, and key mixing

For mathematical details and structure, refer to the documentation.

## ▶️ How to Use

### Compile (g++):
```bash
g++ -o kuznyechik src/skakavac_04.cpp

./kuznyechik
At startup, you will be prompted to choose:

1 – test encryption

2 – test decryption
```

## 🧪 Test Case (from official documentation)
Plaintext: 1122334455667700ffeeddccbbaa9988

Ciphertext: 7f679d90bebc24305a468d42b9d4edcd


## 📄 Documentation
See doc/kuznyechik.pdf for:

- Theoretical background

- GF(2⁸) math and transformations

- Flow diagrams

- Implementation analysis

## 👨‍💻 Author
Name: Pavel Folnović

Mentor: prof. Alen Lovrenčić, Ph.D.
