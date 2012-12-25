/*
 * This proprietary software may be used only as
 * authorised by a licensing agreement from ARM Limited
 * (C) COPYRIGHT 2009 - 2010 ARM Limited
 * ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorised
 * copies and copies may only be made to the extent permitted
 * by a licensing agreement from ARM Limited.
 */

/*
attribute vec4 av4position;
attribute vec3 av3colour;

uniform mat4 mvp;

varying vec3 vv3colour;

void main() {
	vv3colour = av3colour;
	gl_Position = mvp * av4position;
}
*/

// ∂‡º∂Œ∆¿Ì≤‚ ‘
attribute vec4 a_position;
attribute vec2 a_texCoord;
varying vec2 v_texCoord;
void main()
{
	gl_Position = a_position;
	v_texCoord = a_texCoord;
}
