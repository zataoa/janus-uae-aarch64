/*
 * Empty OpenGL entry points for the statically linked SDL 1.2.
 *
 * Copyright 2026 Stevie (zataoa). New file.
 *
 * This file is part of Janus-UAE.
 *
 * Janus-UAE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * Janus-UAE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * The AROS SDL 1.2 port calls these from its CyberGraphX video driver.
 * They normally come from the AROS GL library (Mesa). Native Pi AROS
 * images may not have it, and Janus-UAE does not use OpenGL, so each
 * function reports "not available".
 */

void *glACreateContext() { return 0; }
void  glADestroyContext() { }
int   glAGetConfig() { return -1; }
void *glAGetProcAddress() { return 0; }
void  glAMakeCurrent() { }
void  glASetRast() { }
void  glASwapBuffers() { }
