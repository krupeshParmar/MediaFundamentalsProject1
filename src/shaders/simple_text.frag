#version 410

varying vec2 texpos;
uniform sampler2D tex;
//uniform vec4 color;

void main(){

	gl_FragColor = vec4(1, 1, 1, texture2D(tex, texpos).r) * vec4(0.6, 1, 0.2, 1);
}