#version 330 core

layout(location = 0) in vec4 position;
uniform mat4 m_3d;
uniform mat4 m_rot4d;

void main() {
  vec4 rotated = m_rot4d * position;
  float distance = 2.0f;
  float w = 1.0f / (distance - rotated.w);
  mat4 m_4d;
  m_4d[0] = vec4(w,0.0f,0.0f,0.0f);
  m_4d[1] = vec4(0.0f,w,0.0f,0.0f);
  m_4d[2] = vec4(0.0f,0.0f,w,0.0f);
  m_4d[3] = vec4(0.0f,0.0f,0.0f,0.0f);

  vec4 p4d = m_4d * rotated;
  vec4 p3d = vec4(p4d.xyz, 1.0f);
  vec4 pos = m_3d * p3d;

  gl_Position = pos;
};