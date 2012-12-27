/*
 * This proprietary software may be used only as
 * authorised by a licensing agreement from ARM Limited
 * (C) COPYRIGHT 2009 - 2010 ARM Limited
 * ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorised
 * copies and copies may only be made to the extent permitted
 * by a licensing agreement from ARM Limited.
 */

precision mediump float;

uniform sampler2D s_texture;

varying vec2 v_texCoord;
varying vec4 v_color;

void main()
{
	vec4 texColor = texture2D( s_texture, v_texCoord );
	gl_FragColor = texColor * v_color;
}
