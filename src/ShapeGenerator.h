#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// Pomo?na struktura za vra?anje podataka iz funkcija
struct ShapeData {
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
};

namespace ShapeGenerator
{
    // Generiše podatke za kocku dimenzija 1x1x1 sa centrom u (0,0,0)
    ShapeData createCube()
    {
        ShapeData data;
        data.vertices = {
            // Prednja strana
            {-0.5f, -0.5f,  0.5f}, { 0.5f, -0.5f,  0.5f}, { 0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f},
            // Zadnja strana
            {-0.5f, -0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f}, { 0.5f, -0.5f, -0.5f},
            // Lijeva strana
            {-0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f, -0.5f},
            // Desna strana
            { 0.5f, -0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f}, { 0.5f,  0.5f,  0.5f}, { 0.5f, -0.5f,  0.5f},
            // Gornja strana
            {-0.5f,  0.5f,  0.5f}, { 0.5f,  0.5f,  0.5f}, { 0.5f,  0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f},
            // Donja strana
            {-0.5f, -0.5f,  0.5f}, {-0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f,  0.5f}
        };

        data.indices = {
            0, 1, 2, 0, 2, 3,       // Prednja
            4, 5, 6, 4, 6, 7,       // Zadnja
            8, 9, 10, 8, 10, 11,    // Lijeva
            12, 13, 14, 12, 14, 15, // Desna
            16, 17, 18, 16, 18, 19, // Gornja
            20, 21, 22, 20, 22, 23  // Donja
        };

        // Normale su jednostavne za kocku
        data.normals = {
            {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f},
            {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},
            {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
            {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}
        };

        data.uvs = {
            {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f},
            {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f},
            {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f},
            {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f},
            {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f},
            {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}
        };

        return data;
    }

    // Generiše podatke za valjak (cilindar)
    // height - visina, radius - polupre?nik, sectors - broj segmenata (ivica) kruga
    ShapeData createCylinder(float height, float radius, int sectors)
    {
        ShapeData data;
        const float PI = glm::pi<float>();
        float sectorStep = 2.0f * PI / sectors;

        // ----------- 1. OMOTAČ (OVO VEĆ IMAMO) -----------
        for (int i = 0; i <= sectors; ++i)
        {
            float sectorAngle = i * sectorStep;
            float x = cos(sectorAngle);
            float z = sin(sectorAngle);

            // Vrhovi
            data.vertices.push_back(glm::vec3(x * radius, height / 2.0f, z * radius));  // gornji
            data.vertices.push_back(glm::vec3(x * radius, -height / 2.0f, z * radius)); // donji

            // Normale za omotač gledaju "van"
            data.normals.push_back(glm::vec3(x, 0, z));
            data.normals.push_back(glm::vec3(x, 0, z));

            // UV koordinate
            data.uvs.push_back(glm::vec2((float)i / sectors, 1.0f));
            data.uvs.push_back(glm::vec2((float)i / sectors, 0.0f));
        }

        // Indeksi za omotač
        for (int i = 0; i < sectors; ++i)
        {
            int i1 = i * 2;
            int i2 = i1 + 1;
            int i3 = i1 + 2;
            int i4 = i1 + 3;

            data.indices.push_back(i1); data.indices.push_back(i2); data.indices.push_back(i3);
            data.indices.push_back(i3); data.indices.push_back(i2); data.indices.push_back(i4);
        }

        // ----------- 2. BAZE (GORNJA I DONJA) - NOVI DIO -----------

        // Pošto su vrhovi za baze na istim pozicijama kao vrhovi omotača,
        // ali imaju drugačije normale (gore/dole) i UV-ove (kružne),
        // moramo kreirati NOVE vrhove.

        int baseVertexIndex = (int)data.vertices.size(); // Početni indeks za nove vrhove

        // GORNJA BAZA
        // Svi vrhovi gornje baze imaju normalu (0, 1, 0)
        glm::vec3 topNormal(0.0f, 1.0f, 0.0f);
        // Vrh u centru gornje baze
        data.vertices.push_back(glm::vec3(0, height / 2.0f, 0));
        data.normals.push_back(topNormal);
        data.uvs.push_back(glm::vec2(0.5f, 0.5f)); // UV centar
        int topCenterIndex = baseVertexIndex;
        baseVertexIndex++;

        // Vrhovi po ivici gornje baze
        for (int i = 0; i <= sectors; ++i)
        {
            float sectorAngle = i * sectorStep;
            float x = cos(sectorAngle);
            float z = sin(sectorAngle);
            data.vertices.push_back(glm::vec3(x * radius, height / 2.0f, z * radius));
            data.normals.push_back(topNormal);
            data.uvs.push_back(glm::vec2(x * 0.5f + 0.5f, z * 0.5f + 0.5f)); // Kružni UV-ovi
        }

        // Indeksi za gornju bazu (kao lepeza)
        for (int i = 0; i < sectors; ++i)
        {
            data.indices.push_back(topCenterIndex);
            data.indices.push_back(baseVertexIndex + i);
            data.indices.push_back(baseVertexIndex + i + 1);
        }

        baseVertexIndex += sectors + 1;

        // DONJA BAZA
        // Svi vrhovi donje baze imaju normalu (0, -1, 0)
        glm::vec3 bottomNormal(0.0f, -1.0f, 0.0f);
        // Vrh u centru donje baze
        data.vertices.push_back(glm::vec3(0, -height / 2.0f, 0));
        data.normals.push_back(bottomNormal);
        data.uvs.push_back(glm::vec2(0.5f, 0.5f));
        int bottomCenterIndex = baseVertexIndex;
        baseVertexIndex++;

        // Vrhovi po ivici donje baze
        for (int i = 0; i <= sectors; ++i)
        {
            float sectorAngle = i * sectorStep;
            float x = cos(sectorAngle);
            float z = sin(sectorAngle);
            data.vertices.push_back(glm::vec3(x * radius, -height / 2.0f, z * radius));
            data.normals.push_back(bottomNormal);
            data.uvs.push_back(glm::vec2(x * 0.5f + 0.5f, z * 0.5f + 0.5f));
        }

        // Indeksi za donju bazu (obrnuti redoslijed zbog normale)
        for (int i = 0; i < sectors; ++i)
        {
            data.indices.push_back(bottomCenterIndex);
            data.indices.push_back(baseVertexIndex + i + 1);
            data.indices.push_back(baseVertexIndex + i);
        }

        return data;
    }



    ShapeData createSphere(float radius, int sectors, int stacks)
    {
        ShapeData data;

        const float PI = glm::pi<float>();
        float sectorStep = 2.0f * PI / sectors;
        float stackStep = PI / stacks;
        float sectorAngle, stackAngle;

        for (int i = 0; i <= stacks; ++i)
        {
            stackAngle = PI / 2.0f - i * stackStep; // Ugao od gore (PI/2) ka dole (-PI/2)
            float xy = radius * cos(stackAngle);      // Poluprečnik prstena na trenutnoj visini
            float z = radius * sin(stackAngle);       // Z koordinata (visina) na prstenu

            for (int j = 0; j <= sectors; ++j)
            {
                sectorAngle = j * sectorStep; // Ugao oko Z ose

                // Pozicija vrha (vertex)
                float x = xy * cos(sectorAngle);
                float y = xy * sin(sectorAngle);
                data.vertices.push_back(glm::vec3(x, y, z));

                // Normala (za sferu, normala je ista kao i normalizovani vektor od centra do vrha)
                data.normals.push_back(glm::normalize(glm::vec3(x, y, z)));

                // UV koordinate
                float u = (float)j / sectors;
                float v = (float)i / stacks;
                data.uvs.push_back(glm::vec2(u, v));
            }
        }

        // Generisanje indeksa za trouglove
        // Iscrtavamo četverouglove između svaka 4 susjedna vrha
        for (int i = 0; i < stacks; ++i)
        {
            int k1 = i * (sectors + 1); // Početak trenutnog prstena
            int k2 = k1 + sectors + 1;  // Početak sljedećeg prstena

            for (int j = 0; j < sectors; ++j, ++k1, ++k2)
            {
                // Dva trougla koja čine četverougao
                if (i != 0)
                {
                    data.indices.push_back(k1);
                    data.indices.push_back(k2);
                    data.indices.push_back(k1 + 1);
                }

                if (i != (stacks - 1))
                {
                    data.indices.push_back(k1 + 1);
                    data.indices.push_back(k2);
                    data.indices.push_back(k2 + 1);
                }
            }
        }

        return data;
    }
}