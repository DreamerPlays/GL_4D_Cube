#version 330 core
    
layout(location = 0) out vec4 color;

uniform vec3 u_Color;

vec3 Hue(float H) {
  float R = abs(H * 6 - 3) - 1;
  float G = 2 - abs(H * 6 - 2);
  float B = 2 - abs(H * 6 - 4);
  return vec3(R,G,B);
}

vec4 HSVtoRGB(in vec3 HSV) {
  return vec4(((Hue(HSV.x) - 1) * HSV.y + 1) * HSV.z,1);
}

void main() {
  float r = 0.0;
  vec2 cxy = 2.0f * gl_PointCoord - 1.0f;
  r = dot(cxy, cxy);
  if (r > 1.0f) {
    discard;
  }
  color = HSVtoRGB(u_Color);
};