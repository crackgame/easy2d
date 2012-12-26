/*
 * This proprietary software may be used only as
 * authorised by a licensing agreement from ARM Limited
 * (C) COPYRIGHT 2009 - 2010 ARM Limited
 * ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorised
 * copies and copies may only be made to the extent permitted
 * by a licensing agreement from ARM Limited.
 */

//precision lowp float;
precision mediump float;					  
varying vec2 v_texCoord;                     
uniform sampler2D s_texture;                 
void main()
{
	vec4 texColor;
	texColor = texture2D( s_texture, v_texCoord );
	//if (texColor.a == 0.0)	discard;
	gl_FragColor = texColor;
}
