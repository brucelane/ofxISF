/*
{
  "CATEGORIES": [
    "Automatically Converted"
  ],
  "INPUTS": [
    
  ]
}
*/


#ifdef GL_ES
precision mediump float;
#endif


void main( void ) {

	vec2 position = ( gl_FragCoord.xy / RENDERSIZE.xy );
	vec3 light = vec3(pow(1.0-abs(position.y+cos(position.x*-4.0+TIME)/10.0-0.5),50.0),
			  pow(1.0-abs(position.y-cos(position.x* 4.0+TIME)/10.0-0.5),50.0),
			  pow(1.0-abs(position.y+cos(position.x* 4.0+TIME)/10.0-0.5),50.0));
	light += pow(light.r+light.g+light.b,1.0);
	gl_FragColor = vec4( light.r,light.g,light.b, 1.0);
}