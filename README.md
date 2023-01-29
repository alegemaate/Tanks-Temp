# Tanks
![sonarcloud](https://sonarcloud.io/api/project_badges/measure?project=alegemaate_Tanks&metric=code_smells)
![sonarcloud](https://sonarcloud.io/api/project_badges/measure?project=alegemaate_Tanks&metric=bugs)

A 2D, top down, tank fighting game built using SDL2


![image](https://user-images.githubusercontent.com/9920336/140231515-f7908323-4cda-44f5-91cb-4cbe48ccdb20.png)


## Getting started

### Windows (MSYS2)

#### Install Libraries (MSYS 2)

```bash
https://www.allegro.cc/files/?v=4.4
```

#### Build

```bash
cmake -G "MSYS Makefiles" .
```

```bash
make
```

### Mac OS (Brew)

#### Install Libraries

```bash
https://github.com/msikma/liballeg.4.4.2-osx
```

#### Build

```bash
cmake .
```

```bash
make
```

### Linux

#### Install Libraries

```bash
sudo apt-get install liballegro4-dev libloadpng4-dev liblogg4-dev
```

#### Build

```bash
cmake .
```

```bash
make
```
