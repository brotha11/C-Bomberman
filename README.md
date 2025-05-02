# C-Bomberman
**A Bomberman game made in C**

This is an open-source Bomberman game I'm developing to learn more about the C programming language and SDL2, and also for fun, of course.

---

### Compiling

This project uses *gcc* to compile the project, make sure it's installed in your machine in order to use the provided tools.

#### Using Make (Linux)
This program can be compiled using the provided Makefile.

1. **Install SDL2 (on Ubuntu/Debian):**  
If you're using a Linux distribution such as Ubuntu or Debian, make sure you have SDL2 installed. You can do this by running the following commands:

    ~~~
    sudo apt-get update  
    sudo apt-get install libsdl2-dev
    sudo apt-get install libsdl2-image-dev
    sudo apt-get install libsdl2-mixer-dev
    ~~~

2. **Compiling the game:**  
Once SDL2 is installed, run the following command to compile the program using the included `Makefile`:

  - **Build**

    ~~~
    make
    ~~~

    This will build the project, and you'll be able to run the game.
  
  - **Clean**
    ~~~
    make clean
    ~~~

    Deletes the `obj` directory and all compiled files, including the executable.

  - **CleanObj**
    ~~~
    make cleanobj
    ~~~

    Simpler clean, only removes the `obj` file, leaving the executable.
  
  - **Run**
    ~~~
    make run
    ~~~
    With this option, the program will be executed.

#### Using .bat file (Windows)

On your Windows machine, you can use the provided **build.bat** file to compile the project.

1. **Check files:**
For Windows builds, make sure that the `include`, `lib` folders, and the dll files: *SDL2.dll*, *SDL2_image.dll* and *SDL2_mixer.dll* are present in the root folder.

2. **Compile the game:**
You can put the following commands in a terminal to execute the .bat file:

  - **Build**

    ~~~
    ./build.bat build
    ~~~  
    This will build the project for your Windows machine.
    This option does not erase old compiled files.

  - **Rebuild**

    ~~~
    ./build.bat rebuild
    ~~~  
    Builds the game and erases old compiled files for new ones.

  - **Clean**

    ~~~
    ./build.bat clean
    ~~~  
    This option removes the `obj` folder, including all compiled files.

  - **Run**

    ~~~
    ./build.bat run
    ~~~  
    Will run the executable file if present.

  - **Full**

    ~~~
    ./build.bat full
    ~~~  
    This option will:
    - Clean the `obj` directory
    - Build the game
    - Run the executable after compilation

Aditionally, there is an option present in the .bat file to run the game after a succesfull compilation with any option. 
This option is **disabled** by default. To enable it, simply change the following line in `build.bat`:

  ~~~
  :: Execute game after compiling
  set RUN_AFTER_COMPILE=true
  ~~~


---

### Running the game

After compiling, you can run the game with:

1. **Makefile**
    ~~~
    make run
    ~~~
1. **Build.bat**
    ~~~
    ./build.bat run
    ~~~
3. **Terminal**
    ~~~
    ./C-Bomberman
    ~~~

Make sure you have the necessary assets (sprites, sounds, etc.) in the `res` folder.

---

### Credits

This project is purely for educational and personal purposes, and **is not meant to be sold**. All characters, graphics, sounds, and other protected materials are owned by their respective creators. I do not claim ownership over any trademarks.

#### Sprites
- **The Sprites Resource (Rippers):**
  - Gors
  - T_M

#### Sound effects

- **The Sounds Resource (Rippers):**
  - SteamyJ
  - Phongpon

#### Music

- *梅塔提督 (Admiral Umeta)*
  - Super Bomberman 3 - Battle Theme Mash-UP MIX [https://www.youtube.com/watch?v=YWJUIZqiGBU]