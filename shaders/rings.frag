uniform float res;
uniform vec3 col;
uniform float shift;
uniform float iter;
uniform float zoom;
uniform float dx;
uniform float dy;
uniform float brightness;
uniform vec2 c;
//const vec2 c = vec2(0.73736888,-0.67549029); //constant version
uniform int power;


const float sig = sqrt(3.);
const float phi = (1 + sqrt(5.)) / 2.;

vec2 mult(vec2 a, vec2 b){
    return vec2(a.x *b.x - a.y * b.y, a.x * b.y + a.y * b.x);
}
vec2 div(vec2 x, vec2 y){
    float denom = y.x * y.x + y.y * y.y; 
    float real = (x.x * y.x + x.y * y.y) / denom;
    float imag = (x.y * y.x - x.x * y.y) / denom;
    return vec2(real,imag);
}


void main()
{
    
    float u = (gl_FragCoord.x-shift) / res * 4 - 2.0;
    float v = (gl_FragCoord.y) / res * 4 - 2.0;
    vec2 zSum = vec2(0.,0.);
    vec2 z  = vec2(u,-v);
    z *= zoom;
    z += vec2(dx,-dy);
    bool esc = false;
    float i = 0.0;
    for(; i  < iter; i++){
        zSum += z;
        vec2 cop = z;
        for(int i = 1; i < power; i++){
            cop = mult(cop,z);
        }

        //z = div(mult(mult(cop + vec2(sig,0),cop) , c),(mult(vec2(sig,0),cop) + vec2(1,0)));
        //^break in case of bad code!
        vec2 top = mult(mult(cop, cop + (vec2(sig,0))),c);
        vec2 bot = sig * cop + vec2(1.0,0.0);
        z = div(top,bot);
        
        
    }
    
    float mag = length(zSum);
    float cvv = 1-pow(2.0,-mag);

    gl_FragColor = esc? vec4(col  *  vec3(i / brightness), 1.0) : vec4(0.0,0.0,cvv,1.0);
    //gl_FragColor = esc? vec4(col  *  vec3(i / brightness), 1.0) : vec4(0.0,0.0,0.0,1.0);

}