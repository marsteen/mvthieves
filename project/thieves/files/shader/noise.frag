//
//  Fragment Shader
//

uniform sampler2D uTexture0;


uniform vec2      uNoiseSeed;
uniform vec2      uFboSize;
uniform int       uShaderMode;

float rand(vec2 co)
{
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}


float Noise()
{
  vec3 col;
  vec2 seed;
  
  seed = fract(gl_TexCoord[0].st * uNoiseSeed);
  
  return fract(rand(seed)); // * 0.25;
}


//
//
//

void main() 
{ 
  if (uShaderMode == 0)
  {
    float noise_faktor = 0.8;
    float n1 = Noise() * noise_faktor;
    float n  = 1.0 - n1;
    float n2 = n1 * 0.1;
    vec4 p1  = texture2D(uTexture0, gl_TexCoord[0].st);
    vec4 pixel_n = vec4(p1.r * n + n2, p1.g * n + n2, p1.b * n + n2, p1.a);
    
    //vec4 pixel1 = texture2D(uTexture0, gl_TexCoord[0].st);
    //vec4 pixel1 = texture2D(uTexture1, gl_TexCoord[0].st);
    //color = pixel0;
    //vec3 color = mix(pixel0, pixel1, 0.5);
    
    
    
    
    
    gl_FragColor = pixel_n;
  }
  else
  {
    vec4 pixel1 = texture2D(uTexture0, gl_TexCoord[0].st);
    gl_FragColor = pixel1;
  }  
}
