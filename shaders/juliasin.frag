uniform float res;
uniform vec3 col;
uniform float shift;
uniform float iter;
uniform float zoom;
uniform float dx;
uniform float dy;
uniform float brightness;
uniform vec2 c;
vec2 mult(vec2 a, vec2 b){
    return vec2(a.x *b.x - a.y * b.y, a.x * b.y + a.y * b.x);
}

float cosh(float val)
{
    float tmp = exp(val);
    float cosH = (tmp + 1.0 / tmp) / 2.0;
    return cosH;
}
float sinh(float val)
{
    float tmp = exp(val);
    float sinH = (tmp - 1.0 / tmp) / 2.0;
    return sinH;
}   
vec2 coomplexsin(vec2 a){
	vec2 cop = a;
	cop.x = sin(a.x) * cosh(a.y);
	cop.y = cos(a.x) * sinh(a.y);
	return cop;
}


void main()
{
    
    float u = (gl_FragCoord.x-shift) / res * 4 - 2.0;
    float v = (gl_FragCoord.y) / res * 4 - 2.0;

    vec2 z  = vec2(u,-v);
    z *= zoom;
    z += vec2(dx,-dy);
    bool esc = false;
    float i = 0.0;
    for(; i  < iter; i++){
        z = mult(coomplexsin(z),c);

        if(length(z) >= 50.0){
            esc = true;
            break;
        }
    }
    

    

    gl_FragColor = esc? vec4(col  *  vec3(i / brightness), 1.0) : vec4(0.0,0.0,0.0,1.0);

}