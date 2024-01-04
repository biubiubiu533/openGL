#version 330 core
layout (triangles) in;   //声明输入的图元类型
layout (triangle_strip, max_vertices = 3) out;


in vec2 texCoords[];

out vec2 TexCoord;

void unChange(int index){
    gl_Position = gl_in[index].gl_Position;

    TexCoord = texCoords[index];
    EmitVertex();
}

void main() {
    unChange(0);
    unChange(1);
    unChange(2);
    EndPrimitive();
}