# OpenGl w/ Conan, Cmake

Experimental project, i'll se how it'll ends up 


## 💬 To run this project, Conan 2.0 and Cmake needs to be installed

- First, choose which libraries build type u want:
```bash
conan install . -sbuild_type=Release -of="conan/rel" --build=missing
```
```bash
conan install . -sbuild_type=Debug -of="conan/deb" --build=missing
```
- Then, show the presets to choose from 
```bash
cmake --list-presets 
```
- Choose the build type (choose build type of libraries you've chosen in previous step)
```bash
cmake . --preset <name-of-preset>
```
- Lastly, build the project
```bash
cmake --build --preset "<name-of-preset>"
```


"environment": {
                "PATH": "$env{HOME}/msys64/ucrt64/bin;$penv{PATH}"
            },



