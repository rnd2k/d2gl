#pragma once

/* GLSL Vertex */
"#version 330\n"
"layout(location=0) in vec2 Position;"
"layout(location=1) in vec2 TexCoord;"
"layout(location=5) in ivec4 Flags;"
"uniform mat4 u_MVP;"
"out vec2 v_TexCoord;"
"flat out ivec4 v_Flags;"
"void main()"
"{"
  "gl_Position=u_MVP*vec4(Position,0.,1.),v_TexCoord=TexCoord,v_Flags=Flags;"
"}",

/* GLSL Fragment */
"#version 330\n"
"layout(location=0) out vec4 FragColor;layout(std140) uniform ubo_Metrics{float u_SharpStrength;float u_SharpClamp;float u_Radius;float pad1;vec2 u_RelSize;};"
"uniform sampler2D u_Texture;"
"in vec2 v_TexCoord;"
"flat in ivec4 v_Flags;"
"float t(vec3 v)"
"{"
  "return v.y*(.587/.299)+v.x;"
"}"
"vec3 u(vec3 e)"
"{"
  "vec2 u=v_TexCoord,v=u_RelSize;"
  "vec3 y=texture(u_Texture,u+vec2(0.,-v.y)).xyz,m=texture(u_Texture,u+vec2(-v.x,0.)).xyz,x=e,a=texture(u_Texture,u+vec2(v.x,0.)).xyz,z=texture(u_Texture,u+vec2(0.,v.y)).xyz;"
  "float f=t(y),d=t(m),s=t(x),r=t(a),i=t(z),p=min(s,min(min(f,d),min(i,r))),n=max(s,max(max(f,d),max(i,r))),l=n-p;"
  "if(l<max(.0625,n*.125))"
    "return x;"
  "vec3 b=y+m+x+a+z;"
  "float F=(f+d+r+i)*.25,o=abs(F-s),c=max(0.,o/l-.25)*(1./.75);"
  "c=min(.75,c);"
  "vec3 T=texture(u_Texture,u+vec2(-v.x,-v.y)).xyz,S=texture(u_Texture,u+vec2(v.x,-v.y)).xyz,C=texture(u_Texture,u+vec2(-v.x,v.y)).xyz,h=texture(u_Texture,u+vec2(v.x,v.y)).xyz;"
  "b+=T+S+C+h;"
  "b*=vec3(1./9.);"
  "float R=t(T),g=t(S),M=t(C),D=t(h),k=abs(.25*R+-.5*f+.25*g)+abs(.5*d+-1.*s+.5*r)+abs(.25*M+-.5*i+.25*D),Z=abs(.25*R+-.5*d+.25*M)+abs(.5*f+-1.*s+.5*i)+abs(.25*g+-.5*r+.25*D);"
  "bool Y=Z>=k;"
  "float X=Y?-v.y:-v.x;"
  "if(!Y)"
    "f=d,i=r;"
  "float W=abs(f-s),V=abs(i-s);"
  "f=(f+s)*.5;"
  "i=(i+s)*.5;"
  "if(W<V)"
    "f=i,f=i,W=V,X*=-1.;"
  "vec2 U;"
  "U.x=u.x+(Y?0.:X*.5);"
  "U.y=u.y+(Y?X*.5:0.);"
  "W*=.25;"
  "vec2 Q=U,P=Y?vec2(v.x,0.):vec2(0.,v.y);"
  "float O=f,N=f;"
  "bool L=false,K=false;"
  "U+=P*vec2(-1.,-1.);"
  "Q+=P*vec2(1.,1.);"
  "for(int J=0;J<16;J++)"
    "{"
      "if(!L)"
        "O=t(texture(u_Texture,U.xy).xyz);"
      "if(!K)"
        "N=t(texture(u_Texture,Q.xy).xyz);"
      "L=L||abs(O-f)>=W;"
      "K=K||abs(N-f)>=W;"
      "if(L&&K)"
        "break;"
      "if(!L)"
        "U-=P;"
      "if(!K)"
        "Q+=P;"
    "}"
  "float J=Y?u.x-U.x:u.y-U.y,I=Y?Q.x-u.x:Q.y-u.y;"
  "bool H=J<I;"
  "O=H?O:N;"
  "if(s-f<0.==O-f<0.)"
    "X=0.;"
  "float G=I+J;"
  "J=H?J:I;"
  "float E=(.5+J*(-1./G))*X;"
  "vec3 B=texture(u_Texture,vec2(u.x+(Y?0.:E),u.y+(Y?E:0.))).xyz;"
  "return vec3(-c)*B+(b*vec3(c)+B);"
"}"
"vec3 e(vec3 u)"
"{"
  "vec3 v=texture(u_Texture,v_TexCoord+u_RelSize*u_Radius).xyz,y=texture(u_Texture,v_TexCoord-u_RelSize*u_Radius).xyz,e=(v+y)/2.,i=u-e,f=vec3(.2126,.7152,.0722)*u_SharpStrength*1.5;"
  "vec4 s=vec4(f*(.5/u_SharpClamp),.5);"
  "float x=u_SharpClamp*2.*clamp(dot(vec4(i,1.),s),0.,1.)-u_SharpClamp;"
  "return clamp(u+x,0.,1.);"
"}"
"void main()"
"{"
  "vec3 v=texture(u_Texture,v_TexCoord).xyz;"
  "if(v_Flags.x==0)"
    "FragColor=vec4(e(v),1.);"
  "else "
     "FragColor=vec4(u(v),1.);"
"}",

/* GLSL Compute */
nullptr,