#pragma once

"#ifdef VERTEX\n"
"layout(location=0) in vec2 Position;"
"layout(location=1) in vec2 TexCoord;"
"layout(location=2) in vec4 Color1;"
"layout(location=3) in vec4 Color2;"
"layout(location=4) in uint TexNum;"
"layout(location=5) in uvec4 Flags;"
"layout(location=6) in vec2 Extra;"
"uniform mat4 u_MVP;"
"out vec4 v_Position;"
"out vec2 v_TexCoord;"
"out vec4 v_Color1,v_Color2;"
"flat out uint v_TexNum;"
"flat out uvec4 v_Flags;"
"out vec2 v_Extra;"
"void main()"
"{"
  "v_Position=u_MVP*vec4(Position,0,1);"
  "gl_Position=v_Position;"
  "v_TexCoord=TexCoord;"
  "v_Color1=Color1.wzyx;"
  "v_Color2=Color2.wzyx;"
  "v_TexNum=TexNum;"
  "v_Flags=Flags;"
  "v_Extra=Extra;"
"}"
"\n#elif FRAGMENT\n"
"layout(location=0) out vec4 FragColor;"
"uniform sampler2D u_MapTexture;"
"uniform sampler2DArray u_CursorTexture,u_FontTexture;"
"in vec4 v_Position;"
"in vec2 v_TexCoord;"
"in vec4 v_Color1,v_Color2;"
"flat in uint v_TexNum;"
"flat in uvec4 v_Flags;"
"in vec2 v_Extra;"
"uniform vec2 u_Size,u_Scale;"
"uniform vec4 u_TextMask;"
"uniform bool u_IsMasking=false;"
"float v(vec3 v,float u,float y)"
"{"
  "float r=max(min(v.x,v.y),min(max(v.x,v.y),v.z)),n=max(u,1.)*(r-1.+.5*y);"
  "return clamp(n+.5,0.,1.);"
"}"
"vec3 v(vec3 v,float u)"
"{"
  "float r=dot(v,vec3(.299,.587,.114));"
  "return mix(v,vec3(r),u);"
"}"
"void main()"
"{"
  "switch(v_Flags.x){"
    "case 1u:"
      "FragColor=texture(u_CursorTexture,vec3(v_TexCoord,v_TexNum))*v_Color1;"
      "break;"
    "case 2u:"
      "FragColor=v_Color1;"
      "break;"
    "case 3u:"
      "{"
        "vec3 u=texture(u_FontTexture,vec3(v_TexCoord,v_TexNum)).xyz;"
        "if(v_Flags.w==1u)"
          "{"
            "float r=v(u,v_Extra.x,v_Extra.y+.05),y=v(u,v_Extra.x,.95);"
            "FragColor=vec4(mix(v_Color2.xyz,v_Color1.xyz,y),v_Color1.w*r);"
          "}"
        "else "
           "if(v_Flags.w==2u)"
            "{"
              "float r=v(u,v_Extra.x,v_Extra.y+.1),y=v(u,v_Extra.x,1.1);"
              "FragColor=vec4(mix(v_Color2.xyz,v_Color1.xyz,y),v_Color1.w*r);"
            "}"
          "else "
            "{"
              "float r=v(u,v_Extra.x,v_Extra.y);"
              "FragColor=vec4(v_Color1.xyz,v_Color1.w*r);"
            "}"
        "if(u_IsMasking&&v_Flags.z==0u)"
          "if(u_TextMask.x<v_Position.x&&u_TextMask.z>v_Position.x&&u_TextMask.y>v_Position.y&&u_TextMask.w<v_Position.y)"
            "FragColor.w=0.;"
          "else "
             "FragColor.w*=.7;"
      "}"
      "break;"
    "case 4u:"
      "FragColor=vec4(0);"
      "break;"
    "case 5u:"
      "FragColor=texture(u_MapTexture,v_TexCoord);"
      "FragColor.xyz=v(FragColor.xyz,.2);"
      "break;"
  "}"
  "float u=1.00001;"
  "vec2 d=vec2(u/u_Scale.x/v_Extra.x,u/u_Scale.y/v_Extra.y),r=d*2.,n=d*3.;"
  "switch(v_Flags.y){"
    "case 1u:"
      "if(v_TexCoord.x<d.x||v_TexCoord.x>1.-d.x||v_TexCoord.y<d.y||v_TexCoord.y>1.-d.y)"
        "FragColor=v_Color2;"
    "case 2u:"
      "if(v_TexCoord.x>r.x&&v_TexCoord.x<n.x||v_TexCoord.x<1.-r.x&&v_TexCoord.x>1.-n.x||v_TexCoord.y>r.y&&v_TexCoord.y<n.y||v_TexCoord.y<1.-r.y&&v_TexCoord.y>1.-n.y)"
        "FragColor=v_Color2;"
      "break;"
    "case 3u:"
      "if(v_TexCoord.x<n.x||v_TexCoord.x>=1.-n.x||v_TexCoord.y<n.y||v_TexCoord.y>=1.-n.y)"
        "FragColor=v_Color1;"
      "if(v_TexCoord.x<d.x||v_TexCoord.x>=1.-d.x||v_TexCoord.y<d.y||v_TexCoord.y>=1.-d.y)"
        "FragColor=v_Color2;"
      "if(v_TexCoord.x>=r.x&&v_TexCoord.x<n.x||v_TexCoord.x<1.-r.x&&v_TexCoord.x>=1.-n.x||v_TexCoord.y>=r.y&&v_TexCoord.y<n.y||v_TexCoord.y<1.-r.y&&v_TexCoord.y>=1.-n.y)"
        "FragColor=v_Color2;"
      "break;"
    "case 4u:"
      "if(v_TexCoord.x<d.x||v_TexCoord.x>1.-d.x||v_TexCoord.y<d.y||v_TexCoord.y>1.-d.y)"
        "FragColor=v_Color2;"
      "break;"
  "}"
"}"
"\n#endif"