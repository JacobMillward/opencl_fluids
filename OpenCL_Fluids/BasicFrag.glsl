# version 330 core
uniform sampler2D baseTex;

in Vertex {
	smooth vec4 colour;
	smooth vec2 texCoord;
} IN;

out vec4 gl_FragColor;

void main (void) {
	//vec4 c = texture(baseTex, IN.texCoord);
	gl_FragColor = IN.colour;//mix(IN.colour, c, c.a);
}
