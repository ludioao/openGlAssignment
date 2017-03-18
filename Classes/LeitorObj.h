#pragma once 

#include <string>
#include <vector>

#include <fstream>
#include <sstream>
#define GLM_FORCE_RADIANS
#include <glm/vec3.hpp>

using glm::vec3;
using std::string;
using std::vector;

struct Face
{
    int vInd[3];
    int nInd[3];
    int tInd[3];
};

class LeitorObj
{
public:
    LeitorObj();
    LeitorObj(const string& path);
    void Load(const string& path);

    void Fill(vector<vec3>& data, vector<vec3>& normals, vector<vec3>& colors);

    ~LeitorObj();

    bool SmoothEnabled();
    bool PrecomputedNormals();

private:
    void Init();

    bool vnExist_;
    bool vtExist_;
    bool smoothEnabled_;

    vector<vec3> vertices;
    vector<vec3> normals_;
    vector<Face> faces_;
    vector<vec3> colors_;

};

LeitorObj::LeitorObj()
{
    Init();
}

LeitorObj::LeitorObj(const string& path)
{
    Init();
    Load(path);
}

void LeitorObj::Init()
{
    vnExist_ = false;
    vtExist_ = false;
    smoothEnabled_ = false;
}

LeitorObj::~LeitorObj()
{

}

bool LeitorObj::SmoothEnabled()
{
    return smoothEnabled_;
}

bool LeitorObj::PrecomputedNormals()
{
    return vnExist_;
}

void LeitorObj::Load(const string& path)
{
    std::ifstream input(path);
    if (!input.is_open())
    {
        std::cerr << "can't load model, failure open " << path;
        return;
    }

    std::stringstream stream;

    while (!input.eof())
    {
        std::string line;
        std::getline(input, line);

        if (line[0] == '#')
        {
            std::cout << "Comment : " << line << std::endl;
            continue;

        }

        stream.clear();
        stream.str(line);

        string descriptor;
        stream >> descriptor;

        if (descriptor == "mtllib")
        {
            string libname;
            stream >> libname;

            std::cout << "lib name " << libname << std::endl;

        }
        else if(descriptor == "o")
        {
            string name;
            stream >> name;

            std::cout << "object name " << name <<  std::endl;

        }
        else if (descriptor == "v")
        {
            float x, y, z;
            stream >> x >> y >> z;
            vertices.push_back(glm::vec3(x, y, z));
            colors_.push_back(glm::vec3( 1., 1., 1.));

            //std::cout << "v:"<< x << " " << y << " " << z << " " << std::endl;
        }
        else if (descriptor == "f")
        {
            // in wavefront indecies start from 1
            Face face{};
            if (!vnExist_ && !vtExist_)
            {
                // format v

                for (int i = 0; i < 3; ++i)
                {
                    stream >> face.vInd[i];
                    face.vInd[i]--;
                }

            }
            else if (vnExist_ && !vtExist_)
            {
                char skip;
                // format v//n
                for (int i = 0; i < 3; ++i)
                {
                    stream >> face.vInd[i] >> skip >> skip >> face.nInd[i];
                    face.vInd[i]--;
                    face.nInd[i]--;
                }
            }
            else if (!vnExist_ && vtExist_)
            {
                // format v/t
                for (int i = 0; i < 3; ++i)
                {
                    stream >> face.vInd[i]; stream.ignore(1);
                    stream >> face.tInd[i];

                    face.vInd[i] --;
                    face.tInd[i] --;
                }
            }
            faces_.push_back(face);
        }
        else if (descriptor == "vn")
        {
            vnExist_ = true;
            float x, y, z;
            stream >> x >> y >> z;
            normals_.push_back(vec3(x, y, z));

        }
        else if (descriptor == "vt")
        {
            vtExist_ = true;
        }
        else if (descriptor == "s")
        {
            string isEnabled;
            stream >> isEnabled;
            if (isEnabled == "off")
            {
                smoothEnabled_ = false;
            }
        }
    }

    std::cout << "model loaded " << std::endl;
}

void LeitorObj::Fill(
        vector<vec3>& data,
        vector<vec3>& normals,
        vector<vec3>& colors)
{
    const int VertexPerFace = 3;
    const int VertexCount = faces_.size() * VertexPerFace;

    data.resize(VertexCount);
    normals.resize(VertexCount);
    colors.resize(VertexCount);

    int counter = 0;
    for (Face& face : faces_)
    {
        for (int i = 0; i < 3; ++i)
        {
            vec3& vertex = vertices[face.vInd[i]];
            vec3& normal = normals_[face.nInd[i]];
            vec3 color(1., 1., 1.);//vertices[face.cInd[i]];

            data[counter] = vertex;
            normals[counter] = normal;
            colors[counter] = color;

            ++counter;
        }
    }
}