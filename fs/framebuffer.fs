#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;

const float offset = 1.0 / 800; //正好一个像素大小

void main()
{
    //灰度
    //vec4 result = texture(texture1, TexCoord);
    //float avg = 0.2126 * result.r + 0.7152 * result.g + 0.0722 * result.b;
    //卷积
    vec2 offset[9] = vec2[](
        vec2(-offset, offset), vec2(0, offset), vec2(offset, offset),
        vec2(-offset, 0), vec2(0, 0), vec2(offset, 0),
        vec2(-offset, -offset), vec2(0, -offset), vec2(offset, -offset)
    );
    float kernel[9] = float[](
        -1, -1, -1,
        -1, 9, -1,
        -1, -1, -1
    );
    vec3 sampleTex[9];
    vec3 col = vec3(0.0);
    for (int i = 0; i < 9; i++){
        sampleTex[i] = texture(texture1, TexCoord + offset[i]).rgb;
        col += sampleTex[i] * kernel[i];
    }

    FragColor = vec4(col, 1.0);

}