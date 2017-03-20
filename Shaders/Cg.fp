#version 330 core
out vec4 color;

in vec3 FragPos;
in vec3 Normal;
  
uniform vec3 lightPos; 
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main(){
  
  // Cor resultante.
  //color = ourColor;

  // Ambiente
  float IntensidadeLuz = 0.1f;
  vec3 Ambiente = IntensidadeLuz * lightColor;

  // LightDir.
  vec3 norm = normalize(Normal);
  vec3 DirecaoLuz = normalize(lightPos- FragPos);
  float diff = max(dot(norm, DirecaoLuz), 0.0);
  vec3 Difuso = diff * lightColor;


  // Especular.
  float IntensidadeEspecular = 0.5f;
  vec3 ViewDirection = normalize(viewPos - FragPos);
  vec3 RefDirection = reflect(-DirecaoLuz, norm);
  float Spec = pow(max(dot(ViewDirection, RefDirection), 0.0), 32);
  vec3 Especular = IntensidadeEspecular * Spec * lightColor;

  vec3 result = (Ambiente + Difuso + Especular) * objectColor;
  color = vec4(result, 1.0f);
}