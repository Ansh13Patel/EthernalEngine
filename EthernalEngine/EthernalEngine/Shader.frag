"version 330 core\n"
"in vec2 TexCoords;\n"
"in vec4 vertexColor;\n"
"out vec4 FragColor;\n"
"uniform sampler2D image;\n"
"void main()\n"
"{\n"
"   color = vertexColor * texture(image, TexCoords);\n"
"}\0";