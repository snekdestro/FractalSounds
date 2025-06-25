uniform float res;
uniform vec3 col;
uniform float shift;
uniform float iter;
uniform float zoom;
uniform float dx;
uniform float dy;
uniform float brightness;

vec2 vecAb(vec2 a){
    return vec2(abs(a.x), abs(a.y));
}

vec2 mult(vec2 a, vec2 b){
    a = vecAb(a);
    b = vecAb(b);
    return vec2(a.x *b.x - a.y * b.y, a.x * b.y + a.y * b.x);
}


void main()
{
    
    float u = (gl_FragCoord.x-shift) / res * 4 - 2.0;
    float v = (gl_FragCoord.y) / res * 4 - 2.0;
    vec2 z = vec2(0.0,0.0);
    vec2 c  = vec2(u,-v);
    c *= zoom;
    c += vec2(dx,-dy);
    bool esc = false;
    float i = 0.0;
    for(; i  < iter; i++){
        z = mult(z,z);
        z = z + c;
        if(length(z) >= 2.0){
            esc = true;
            break;
        }
    }
    

    

    gl_FragColor = esc? vec4(col  *  vec3(i / brightness), 1.0) : vec4(0.0,0.0,0.0,1.0);

}