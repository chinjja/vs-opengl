#include "ObjectLoader.h"

#include <cstdio>
#include <regex>
#include <string>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

bool ObjectLoader::load(const char* filename, std::vector<glm::vec3>& vertex_data, std::vector<glm::vec3>& normal_data, std::vector<glm::vec2>& uv_data)
{
    FILE* file;
    fopen_s(&file, filename, "r");
    if (!file) {
        cout << "can't open file: " << filename << endl;
        return false;
    }

    string text;
    char buf[4096];
    while (!feof(file))
    {
        size_t red_size = fread(buf, 1, sizeof(buf), file);
        text += string(buf, red_size);
    }
    fclose(file);
    if (text[text.size() - 1] != '\n') text += '\n';

    static string space_pattern = "\\s+";
    static string real_pattern = "([+-]?\\d+\\.?\\d*|[+-]?\\d+\\.\\d+e[+-]\\d+)";
    static string space_real_pattern = space_pattern + real_pattern;
    static string vector_pattern = "\\(\\s*" + real_pattern + space_pattern + real_pattern + space_pattern + real_pattern + "\\s*\\)";
    static string index_pattern = "(\\d*)/(\\d*)/(\\d*)";
    static string face_pattern = space_pattern + index_pattern + space_pattern + index_pattern + space_pattern + index_pattern;

    regex v("v" + space_real_pattern + space_real_pattern + space_real_pattern);
    regex vt("vt" + space_real_pattern + space_real_pattern);
    regex vn("vn" + space_real_pattern + space_real_pattern + space_real_pattern);
    regex f("f" + face_pattern);
    smatch m;

    vector<vec3> vertex;
    vector<vec2> uv;
    vector<vec3> normal;

    string::iterator beg = text.begin();
    string::iterator end = text.begin();
    while (end != text.end()) {
        char c = *end;
        end++;
        if (c == '\n') {
            string input(beg, end-1);
            beg = end;

            if (input.empty()) continue;
            if (input.rfind("v ", 0) == 0) {
                if (regex_match(input, m, v)) {
                    vertex.push_back({ stof(m[1]), stof(m[2]), stof(m[3]) });
                }
            }
            else if (input.rfind("vt ", 0) == 0) {
                if (regex_match(input, m, vt)) {
                    uv.push_back({ stof(m[1]), stof(m[2]) });
                }
            }
            else if (input.rfind("vn ", 0) == 0) {
                if (regex_match(input, m, vn)) {
                    normal.push_back({ stof(m[1]), stof(m[2]), stof(m[3]) });
                }
            }
            else if (input.rfind("f ", 0) == 0) {
                if (regex_match(input, m, f)) {
                    for (int i = 1; i < 10; i++) {
                        if (m[i].str().empty()) continue;

                        int idx = stoi(m[i]) - 1;
                        switch (i % 3) {
                        case 1:
                            vertex_data.push_back(vertex[idx]);
                            break;
                        case 2:
                            uv_data.push_back(uv[idx]);
                            break;
                        case 0:
                            normal_data.push_back(normal[idx]);
                            break;
                        }
                    }
                }
            }
        }
    }
    
}

shared_ptr<Mesh> ObjectLoader::load(const char* filename)
{
    vector<vec3> vertex;
    vector<vec2> uv;
    vector<vec3> normal;
    if (load(filename, vertex, normal, uv)) {
        return shared_ptr<Mesh>(new Mesh(vertex, normal, uv));
    }
    return shared_ptr<Mesh>();
}
