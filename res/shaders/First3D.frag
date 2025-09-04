//Created by IDz on 20240116
#version 410

in vec3 fragTexCoord;
out vec4 fragColor;

void main() 
{
    fragColor = vec4(fragTexCoord, 1.0);
}