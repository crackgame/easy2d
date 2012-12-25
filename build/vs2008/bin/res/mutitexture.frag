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
precision lowp float;

varying vec3 vv3colour;

void main() {
	gl_FragColor = vec4(vv3colour, 1.0);
}
*/


// ¶à¼¶ÎÆÀí²âÊÔ
precision mediump float;					  
varying vec2 v_texCoord;                     
uniform sampler2D s_baseMap;                 
uniform sampler2D s_lightMap;                
void main()
{
	vec4 baseColor;
	vec4 lightColor;

	baseColor = texture2D( s_baseMap, v_texCoord );
	lightColor = texture2D( s_lightMap, v_texCoord );
	gl_FragColor = baseColor * (lightColor + 0.25);

	//if (baseColor.a == 0.0 && lightColor.a == 0.0)
	//	discard;

/*
	if (lightColor.r > 0.0)
		gl_FragColor.r = lightColor.r;
	else
		gl_FragColor.r = baseColor.r;

	if (lightColor.g > 0.0)
		gl_FragColor.g = lightColor.g;
	else
		gl_FragColor.g = baseColor.g;

	if (lightColor.b > 0.0)
		gl_FragColor.b = lightColor.b;
	else
		gl_FragColor.b = baseColor.b;

	gl_FragColor.a = baseColor.a + lightColor.a;
*/

//	gl_FragColor = baseColor;
}
