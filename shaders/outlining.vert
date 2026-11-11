#version 330 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;


uniform mat4 camMatrix;
uniform mat4 model;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;
uniform float outlining;

void main(){
    gl_Position = camMatrix * model * translation * rotation * scale * vec4(aPos * aNormal * outlining, 1.0);
}
