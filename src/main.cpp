#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <windows.h>


sf::Vector2f conv(sf::Vector2f UV,int x, int y, float zoom, float dx, float dy){
    float u = ((UV.x-dx)  / zoom + 2.0) / 4.0 * std::min(x,y)  + std::min(x,y) / 2.0;
    
    float v = ((UV.y+dy) / zoom + 2.0) / 4.0 * std::min(x,y);
   
    return sf::Vector2f(u,v);    
}
sf::Vector2f vecAbs(sf::Vector2f vec){
    return sf::Vector2f(abs(vec.x), abs(vec.y));
}

sf::Vector2f mult(sf::Vector2f a, sf::Vector2f b){
    return sf::Vector2f(a.x * b.x - b.y * a.y, a.x * b.y + a.y * b.x);
}
float len(sf::Vector2f a){
    return std::sqrt(a.x * a.x + a.y * a.y);
}


int main()
{
    sf::Vector2f currentC(0,0);
    bool shifting = false;
    sf::Shader shaders[3];
    shaders[0].loadFromFile("shaders/mandel.frag",sf::Shader::Fragment);
    shaders[1].loadFromFile("shaders/burningship.frag", sf::Shader::Fragment);
    shaders[2].loadFromFile("shaders/julia.frag", sf::Shader::Fragment);
    float brightness[3];
    int mode = 0;
    const int bufSize = 4096;
    const double sample_rate = 44100.0;
    const int max_freq = 4000;
    const double volume = 8000.0;
    const double decay = 0.990;
    float zoom[3];
    float dx[3];
    float dy[3];
    auto x = sf::VideoMode::getDesktopMode().width;
    auto y = sf::VideoMode::getDesktopMode().height;
    sf::RenderWindow window(sf::VideoMode(x, y,32), "Fractal Sounds");
    sf::RectangleShape shape(sf::Vector2f(x,y));
    //shape.setFillColor(sf::Color::Green);
    for(int i = 0; i < 3; i++){
        brightness[i] = 1000.;
        zoom[i] = 1;
        dx[i] = 0;
        dy[i] = 0;
        shaders[i].setParameter("res", (float)std::min(x,y));
        shaders[i].setParameter("col", sf::Vector3f(1.0,0.0,0.0));
        shaders[i].setParameter("shift", (float)std::min(x,y) / 2.0);
        shaders[i].setParameter("iter",(float)bufSize / 2.0);
        shaders[i].setParameter("zoom",zoom[i]);
        shaders[i].setParameter("dx",dx[i]);
        shaders[i].setParameter("dy",dy[i]);
        shaders[i].setParameter("brightness", brightness[i]);
    }
    shaders[2].setParameter("c", sf::Vector2f(0,0));
    bool cal = false;
    sf::VertexArray lines(sf::LinesStrip, bufSize / 2);
    std::vector<sf::Int16> samp(bufSize);
    sf::Sound sound;
    sf::SoundBuffer buf;
    
    
    while (window.isOpen())
    {
        sf::Event event;
        
        
        while (window.pollEvent(event))
        {
            
            if (event.type == sf::Event::Closed){
                window.close();
            }
           
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                if(shifting && mode > 1){
                    cal = false;
                    float u = (float)sf::Mouse::getPosition(window).x;
                    float v = (float)sf::Mouse::getPosition(window).y;
                    currentC = sf::Vector2f(((u - (float)std::min(x,y)/2.0)  / std::min(x,y) * 4.0 - 2.0),(v / std::min(x,y) * 4.0 - 2.0));
                    shaders[mode].setUniform("c", currentC);
                }else{
                    cal = true;
                    float u = (float)sf::Mouse::getPosition(window).x;
                    float v = (float)sf::Mouse::getPosition(window).y;
                    sf::Vector2f UV = sf::Vector2f(((u - (float)std::min(x,y)/2.0)  / std::min(x,y) * 4.0 - 2.0),(v / std::min(x,y) * 4.0 - 2.0));
                    
                    UV *= zoom[mode];
                    UV += sf::Vector2f(dx[mode],-dy[mode]);
                    if(mode < 2){
                        sf::Vector2f z = sf::Vector2f(0.0f,0.0f);
                        
                        int idx = 0;
                        int steps = sample_rate / max_freq;
                        for(int i =0; i  < bufSize / 2; i++){
                            if(len(z) > 2){
                                cal = false;
                                break;
                            }
                            int j = i % steps;
                            double t = double(j) / double(steps);
                            t = 0.5 - 0.5*std::cos(t * 3.14159);
                            double dpx = z.x;
                            double dpy = z.y;
                            double m = dpx * dpx + dpy * dpy;
                            if(m > 2.0){
                                dpx *= 2.0 / m;
                                dpy *= 2.0 / m;
                            }
                            samp[idx] = (sf::Int16)std::min(std::max(dpx * t * volume, -32000.0),32000.0);
                            samp[idx + 1] =(sf::Int16)std::min(std::max(dpy * t * volume, -32000.0),32000.0);
                            lines[i] = conv(z,x,y,zoom[mode],dx[mode],dy[mode]);
                            lines[i].color = sf::Color::Red;
                            if(mode != 1){
                                z = mult(z,z) + UV;
                            }else{
                                z = mult(vecAbs(z),vecAbs(z)) + UV;
                            }

                            idx += 2;
                            
                        }
                    }else{
                        sf::Vector2f z = UV;
                        
                        int idx = 0;
                        int steps = sample_rate / max_freq;
                        for(int i =0; i  < bufSize / 2; i++){
                            if(len(z) > 2){
                                cal = false;
                                break;
                            }
                            int j = i % steps;
                            double t = double(j) / double(steps);
                            t = 0.5 - 0.5*std::cos(t * 3.14159);
                            double dpx = z.x;
                            double dpy = z.y;
                            double m = dpx * dpx + dpy * dpy;
                            if(m > 2.0){
                                dpx *= 2.0 / m;
                                dpy *= 2.0 / m;
                            }
                            samp[idx] = (sf::Int16)std::min(std::max(dpx * t * volume, -32000.0),32000.0);
                            samp[idx + 1] =(sf::Int16)std::min(std::max(dpy * t * volume, -32000.0),32000.0);
                            lines[i] = conv(z,x,y,zoom[mode],dx[mode],dy[mode]);
                            lines[i].color = sf::Color::Red;
                            z = mult(z,z) + currentC;
                            idx += 2;
                            
                        }
                    }
                }
                    
                
            }
        
            
            if(event.type == sf::Event::MouseWheelMoved){
                zoom[mode] *= std::pow(2.0,-event.mouseWheel.delta);
                shaders[mode].setParameter("zoom",zoom[mode]);
                cal = false;
            }
            if(event.type == sf::Event::KeyReleased){
                if(!event.key.shift){
                    shifting = false;
                }
            }
            if(event.type == sf::Event::KeyPressed){
                if(event.key.code == sf::Keyboard::Left){
                    dx[mode] -= 0.1 * zoom[mode] * 4.0;
                    cal = false;
                }
                if(event.key.code == sf::Keyboard::Right){
                    dx[mode] += 0.1 * zoom[mode]  * 4.0;
                    cal = false;
                }
                if(event.key.code == sf:: Keyboard::Down){
                    dy[mode] -= 0.1 * zoom[mode] * 4.0;
                    cal = false;
                }
                if(event.key.code == sf:: Keyboard::Up){
                    dy[mode] += 0.1 * zoom[mode] * 4.0;
                    cal = false;
                }
                shaders[mode].setParameter("dx",dx[mode]);
                shaders[mode].setParameter("dy",dy[mode]);
                if(event.key.code == sf::Keyboard::Num1){
                    if(mode != 0){
                        mode =0;
                        
                        cal = false;
                    }
                }
                if(event.key.code == sf::Keyboard::Num2){
                    if(mode != 1){
                        mode = 1;
                        cal = false;
                    }
                }
                if(event.key.code == sf::Keyboard::Num3){
                    if(mode != 2){
                        mode = 2;
                        cal = false;
                    }
                }
                
                if(event.key.code == sf::Keyboard::Equal){
                    brightness[mode] /= 2.;
                    shaders[mode].setUniform("brightness", brightness[mode]);
                }
                if(event.key.code == sf::Keyboard::Dash){
                    brightness[mode] *= 2.;
                    shaders[mode].setUniform("brightness", brightness[mode]);
                }
                if(event.key.shift){
                    shifting = true;
                }
                
            }
        }

        window.clear();
        window.draw(shape, &shaders[mode]);
        if(cal){
           window.draw(lines);
           buf.loadFromSamples(&samp[0],bufSize,2,44100 );
            sound.setBuffer(buf);
            sound.play();
            for(int i = 0; i < bufSize; i++){
                samp[i] = (sf::Int16)(decay * samp[i]);
            }
        }
        window.display();
    }
    return 0;
}

