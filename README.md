Run on terminal, after installing OpenGL, GLUT and pyOpenGL binding.


```
$ python lsystemOpenGL.py
```

Use following keys to play:

+-------------+-----------------+
| Key         |  Description    |
+-------------+-----------------+
| up-arrow    | increase depth  |
| down-arrow  | decrease depth  |
| left-arrow  | zoom in         |
| right-arrow | zoom out        |
| F1          | next file       |
| F2          | prev file       |
+-------------+-----------------+

Compile mandelbrot.c and run the executable

```
$ gcc -o mandelbrot mandelbrot.c -lGL -lGLU -lglut
$ ./mandelbrot
```
