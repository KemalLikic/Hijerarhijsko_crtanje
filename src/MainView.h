#pragma once
#include <gui/gl/View.h>
#include <gui/Key.h>
#include "ShapeGenerator.h"
#include <glm/gtc/type_ptr.hpp> // Za glm::value_ptr
#include <glm/gtc/constants.hpp> // Za glm::pi

// Struktura za jedan dio tijela, samo sa bojom
struct ModelComponent
{
    GLuint vao = 0;
    GLuint vbo_vertices = 0;
    GLuint vbo_normals = 0;
    GLuint ebo = 0;
    td::UINT4 indicesCount = 0;
    glm::vec4 color;
};

class MainView : public gui::gl::View
{
    glm::mat4 _perspectiveMatrix;
    glm::mat4 _viewMatrix;

    td::UINT4 _mainPassProgramID;
    GLuint _matrixID;
    GLuint _objectColorID;

    // Komponente za humanoida
    ModelComponent _trup, _glava, _lijevaNogaGore, _lijevaNogaDole, _desnaNogaGore, _desnaNogaDole,
        _lijevaRukaGore, _lijevaRukaDole, _desnaRukaGore, _desnaRukaDole;
    ModelComponent _lijevoRame, _desnoRame, _lijeviLakat, _desniLakat,
        _lijeviKuk, _desniKuk, _lijevoKoljeno, _desnoKoljeno;

    // Varijable za animaciju i kretanje
    float _ugaoHoda = 0.0f;
    float _brzinaHoda = 4.0f;
    bool _kretanjeNaprijed = false, _kretanjeNazad = false, _rotacijaLijevo = false, _rotacijaDesno = false;
    float _humanoidPozicijaX = 0.0f, _humanoidPozicijaZ = 0.0f;
    float _humanoidUgaoRotacije = glm::pi<float>();
    float _humanoidBrzinaKretanja = 0.1f, _humanoidBrzinaRotacije = 0.05f;

private:
    bool setupShaders()
    {
        // Koristimo naše nove, super-jednostavne šejdere
        _mainPassProgramID = linkProgram(":simpleV", ":simpleF");
        assert(_mainPassProgramID > 0);
        return true;
    }

    void createModelComponent(const ShapeData& shapeData, const glm::vec4& color, ModelComponent& component)
    {
        component.color = color;
        component.indicesCount = (td::UINT4)shapeData.indices.size();

        glGenVertexArrays(1, &component.vao);
        glBindVertexArray(component.vao);

        glGenBuffers(1, &component.vbo_vertices);
        glBindBuffer(GL_ARRAY_BUFFER, component.vbo_vertices);
        glBufferData(GL_ARRAY_BUFFER, shapeData.vertices.size() * sizeof(glm::vec3), shapeData.vertices.data(), GL_STATIC_DRAW);

        glGenBuffers(1, &component.vbo_normals);
        glBindBuffer(GL_ARRAY_BUFFER, component.vbo_normals);
        glBufferData(GL_ARRAY_BUFFER, shapeData.normals.size() * sizeof(glm::vec3), shapeData.normals.data(), GL_STATIC_DRAW);

        glGenBuffers(1, &component.ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, component.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapeData.indices.size() * sizeof(unsigned int), shapeData.indices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    void drawComponent(const ModelComponent& component, const glm::mat4& modelMatrix)
    {
        glm::mat4 MVP = _perspectiveMatrix * _viewMatrix * modelMatrix;
        glUniformMatrix4fv(_matrixID, 1, GL_FALSE, glm::value_ptr(MVP));
        glUniform4f(_objectColorID, component.color.r, component.color.g, component.color.b, component.color.a);

        glBindVertexArray(component.vao);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, component.vbo_vertices);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // Omogućavamo i normale, iako ih šejder ne koristi, da ne bi došlo do greške
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, component.vbo_normals);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, component.ebo);
        glDrawElements(GL_TRIANGLES, component.indicesCount, GL_UNSIGNED_INT, (void*)0);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glBindVertexArray(0);
    }

    void setup()
    {
        const glm::vec4 bojaTrupa(0.0f, 0.0f, 1.0f, 1.0f);   // Plava
        const glm::vec4 bojaGlave(1.0f, 0.8f, 0.6f, 1.0f);   // Boja kože
        const glm::vec4 bojaUdova(1.0f, 0.0f, 0.0f, 1.0f);   // Crvena
        const glm::vec4 bojaZglobova(1.0f, 1.0f, 0.0f, 1.0f); // Žuta

        ShapeData valjakData = ShapeGenerator::createCylinder(1.0f, 0.5f, 16);
        ShapeData kockaData = ShapeGenerator::createCube();
        ShapeData sferaData = ShapeGenerator::createSphere(1.0f, 24, 12);
        ShapeData loptaData = ShapeGenerator::createSphere(1.0f, 32, 16);

        createModelComponent(valjakData, bojaTrupa, _trup);
        createModelComponent(loptaData, bojaGlave, _glava);
        createModelComponent(kockaData, bojaUdova, _lijevaNogaGore);
        createModelComponent(kockaData, bojaUdova, _lijevaNogaDole);
        createModelComponent(kockaData, bojaUdova, _desnaNogaGore);
        createModelComponent(kockaData, bojaUdova, _desnaNogaDole);
        createModelComponent(kockaData, bojaUdova, _lijevaRukaGore);
        createModelComponent(kockaData, bojaUdova, _lijevaRukaDole);
        createModelComponent(kockaData, bojaUdova, _desnaRukaGore);
        createModelComponent(kockaData, bojaUdova, _desnaRukaDole);
        createModelComponent(sferaData, bojaZglobova, _lijevoRame);
        createModelComponent(sferaData, bojaZglobova, _desnoRame);
        createModelComponent(sferaData, bojaZglobova, _lijeviLakat);
        createModelComponent(sferaData, bojaZglobova, _desniLakat);
        createModelComponent(sferaData, bojaZglobova, _lijeviKuk);
        createModelComponent(sferaData, bojaZglobova, _desniKuk);
        createModelComponent(sferaData, bojaZglobova, _lijevoKoljeno);
        createModelComponent(sferaData, bojaZglobova, _desnoKoljeno);

        _matrixID = glGetUniformLocation(_mainPassProgramID, "MVP");
        _objectColorID = glGetUniformLocation(_mainPassProgramID, "objectColor");
    }

protected:
    void onInit() override
    {
        if (!setupShaders()) {
            mu::dbgLog("ERROR! OpenGL cannnot setup shaders!");
            assert(false);
            return;
        }

        float fov = 90.0f;
        float aspectRatio = 16.0f / 9.0f;
        float nearClip = 0.1f;
        float farClip = 100.0f;
        _perspectiveMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        setup();
        setFocus();
    }

    bool prepareNextFrame() override
    {
        if (_kretanjeNaprijed || _kretanjeNazad) {
            _ugaoHoda += 0.05f * _brzinaHoda;
        }

        if (_rotacijaLijevo) _humanoidUgaoRotacije += _humanoidBrzinaRotacije;
        if (_rotacijaDesno) _humanoidUgaoRotacije -= _humanoidBrzinaRotacije;

        float moveStep = 0.0f;
        if (_kretanjeNaprijed) moveStep += _humanoidBrzinaKretanja;
        if (_kretanjeNazad) moveStep -= _humanoidBrzinaKretanja;

        _humanoidPozicijaX += moveStep * -glm::cos(_humanoidUgaoRotacije);
        _humanoidPozicijaZ += moveStep * glm::sin(_humanoidUgaoRotacije);

        return true;
    }

    void onDraw(const gui::Rect& rect) override
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(_mainPassProgramID);

        // Fiksna kamera za testiranje
        glm::vec3 cameraPosition = glm::vec3(0, 5, 15);
        glm::vec3 cameraTarget = glm::vec3(0, 1.5, 0);
        _viewMatrix = glm::lookAt(cameraPosition, cameraTarget, glm::vec3(0, 1, 0));

        // Animacijski uglovi
        float animUgao = _ugaoHoda;
        float ugaoHoda = sin(animUgao) * 0.8f;
        float ugaoKoljena = glm::max(0.0f, -sin(animUgao) * 1.5f);
        float ugaoKoljenaSuprotno = glm::max(0.0f, sin(animUgao) * 1.5f);

        // Glavna matrica za poziciju i rotaciju cijelog humanoida
        glm::mat4 humanoidMatrix = glm::mat4(1.0f);
        humanoidMatrix = glm::translate(humanoidMatrix, glm::vec3(_humanoidPozicijaX, 0.0f, _humanoidPozicijaZ));
        humanoidMatrix = glm::rotate(humanoidMatrix, _humanoidUgaoRotacije - glm::pi<float>() / 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));

        // --- HIJERARHIJSKO CRTANJE ---

        // 1. TRUP
        glm::mat4 pozicijaTrupaMatrix = glm::translate(humanoidMatrix, glm::vec3(0.0f, 1.5f, 0.0f));
        glm::mat4 trupZaCrtanjeMatrix = glm::scale(pozicijaTrupaMatrix, glm::vec3(1.0f, 1.7f, 0.75f));
        drawComponent(_trup, trupZaCrtanjeMatrix);

        // 2. GLAVA
        glm::mat4 glavaMatrix = glm::translate(pozicijaTrupaMatrix, glm::vec3(0.0f, 1.1f, 0.0f));
        glavaMatrix = glm::scale(glavaMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
        drawComponent(_glava, glavaMatrix);

        // ------------------- LIJEVA NOGA -------------------
        glm::mat4 pozicijaKukaLijevo = glm::translate(pozicijaTrupaMatrix, glm::vec3(-0.35f, -0.85f, 0.0f));
        glm::mat4 rotacijaKukaLijevo = glm::rotate(pozicijaKukaLijevo, ugaoHoda, glm::vec3(1.0f, 0.0f, 0.0f));
        drawComponent(_lijeviKuk, glm::scale(rotacijaKukaLijevo, glm::vec3(0.15f)));

        glm::mat4 lijevaNadkoljenica_neskalirana = glm::translate(rotacijaKukaLijevo, glm::vec3(0.0f, -0.6f, 0.0f));
        glm::mat4 lijevaNadkoljenica_skalirana = glm::scale(lijevaNadkoljenica_neskalirana, glm::vec3(0.25f, 0.6f, 0.3f));
        drawComponent(_lijevaNogaGore, lijevaNadkoljenica_skalirana);

        glm::mat4 pozicijaKoljenaLijevo = glm::translate(lijevaNadkoljenica_neskalirana, glm::vec3(0.0f, -1.2f, 0.0f)); // Pozicija koljena zavisi od neskalirane nadkoljenice
        glm::mat4 rotacijaKoljenaLijevo = glm::rotate(pozicijaKoljenaLijevo, ugaoKoljena, glm::vec3(1.0f, 0.0f, 0.0f));
        drawComponent(_lijevoKoljeno, glm::scale(rotacijaKoljenaLijevo, glm::vec3(0.12f)));

        glm::mat4 lijevaPotkoljenica = glm::translate(rotacijaKoljenaLijevo, glm::vec3(0.0f, -0.6f, 0.0f));
        lijevaPotkoljenica = glm::scale(lijevaPotkoljenica, glm::vec3(0.22f, 0.6f, 0.27f));
        drawComponent(_lijevaNogaDole, lijevaPotkoljenica);

        // ------------------- DESNA NOGA -------------------
        glm::mat4 pozicijaKukaDesno = glm::translate(pozicijaTrupaMatrix, glm::vec3(0.35f, -0.85f, 0.0f));
        glm::mat4 rotacijaKukaDesno = glm::rotate(pozicijaKukaDesno, -ugaoHoda, glm::vec3(1.0f, 0.0f, 0.0f));
        drawComponent(_desniKuk, glm::scale(rotacijaKukaDesno, glm::vec3(0.15f)));

        glm::mat4 desnaNadkoljenica_neskalirana = glm::translate(rotacijaKukaDesno, glm::vec3(0.0f, -0.6f, 0.0f));
        glm::mat4 desnaNadkoljenica_skalirana = glm::scale(desnaNadkoljenica_neskalirana, glm::vec3(0.25f, 0.6f, 0.3f));
        drawComponent(_desnaNogaGore, desnaNadkoljenica_skalirana);

        glm::mat4 pozicijaKoljenaDesno = glm::translate(desnaNadkoljenica_neskalirana, glm::vec3(0.0f, -1.2f, 0.0f));
        glm::mat4 rotacijaKoljenaDesno = glm::rotate(pozicijaKoljenaDesno, ugaoKoljenaSuprotno, glm::vec3(1.0f, 0.0f, 0.0f));
        drawComponent(_desnoKoljeno, glm::scale(rotacijaKoljenaDesno, glm::vec3(0.12f)));

        glm::mat4 desnaPotkoljenica = glm::translate(rotacijaKoljenaDesno, glm::vec3(0.0f, -0.6f, 0.0f));
        desnaPotkoljenica = glm::scale(desnaPotkoljenica, glm::vec3(0.22f, 0.6f, 0.27f));
        drawComponent(_desnaNogaDole, desnaPotkoljenica);

        // ------------------- LIJEVA RUKA -------------------
        glm::mat4 pozicijaRamenaLijevo = glm::translate(pozicijaTrupaMatrix, glm::vec3(-0.65f, 0.7f, 0.0f));
        glm::mat4 rotacijaRamenaLijevo = glm::rotate(pozicijaRamenaLijevo, -ugaoHoda, glm::vec3(1.0f, 0.0f, 0.0f));
        drawComponent(_lijevoRame, glm::scale(rotacijaRamenaLijevo, glm::vec3(0.18f)));

        glm::mat4 lijevaNadlaktica_neskalirana = glm::translate(rotacijaRamenaLijevo, glm::vec3(0.0f, -0.5f, 0.0f));
        glm::mat4 lijevaNadlaktica_skalirana = glm::scale(lijevaNadlaktica_neskalirana, glm::vec3(0.2f, 0.5f, 0.25f));
        drawComponent(_lijevaRukaGore, lijevaNadlaktica_skalirana);

        glm::mat4 pozicijaLaktaLijevo = glm::translate(lijevaNadlaktica_neskalirana, glm::vec3(0.0f, -1.0f, 0.0f));
        glm::mat4 rotacijaLaktaLijevo = glm::rotate(pozicijaLaktaLijevo, ugaoKoljenaSuprotno, glm::vec3(1.0f, 0.0f, 0.0f));
        drawComponent(_lijeviLakat, glm::scale(rotacijaLaktaLijevo, glm::vec3(0.14f)));

        glm::mat4 lijevaPodlaktica = glm::translate(rotacijaLaktaLijevo, glm::vec3(0.0f, -0.5f, 0.0f));
        lijevaPodlaktica = glm::scale(lijevaPodlaktica, glm::vec3(0.18f, 0.5f, 0.22f));
        drawComponent(_lijevaRukaDole, lijevaPodlaktica);

        // ------------------- DESNA RUKA -------------------
        glm::mat4 pozicijaRamenaDesno = glm::translate(pozicijaTrupaMatrix, glm::vec3(0.65f, 0.7f, 0.0f));
        glm::mat4 rotacijaRamenaDesno = glm::rotate(pozicijaRamenaDesno, ugaoHoda, glm::vec3(1.0f, 0.0f, 0.0f));
        drawComponent(_desnoRame, glm::scale(rotacijaRamenaDesno, glm::vec3(0.18f)));

        glm::mat4 desnaNadlaktica_neskalirana = glm::translate(rotacijaRamenaDesno, glm::vec3(0.0f, -0.5f, 0.0f));
        glm::mat4 desnaNadlaktica_skalirana = glm::scale(desnaNadlaktica_neskalirana, glm::vec3(0.2f, 0.5f, 0.25f));
        drawComponent(_desnaRukaGore, desnaNadlaktica_skalirana);

        glm::mat4 pozicijaLaktaDesno = glm::translate(desnaNadlaktica_neskalirana, glm::vec3(0.0f, -1.0f, 0.0f));
        glm::mat4 rotacijaLaktaDesno = glm::rotate(pozicijaLaktaDesno, ugaoKoljena, glm::vec3(1.0f, 0.0f, 0.0f));
        drawComponent(_desniLakat, glm::scale(rotacijaLaktaDesno, glm::vec3(0.14f)));

        glm::mat4 desnaPodlaktica = glm::translate(rotacijaLaktaDesno, glm::vec3(0.0f, -0.5f, 0.0f));
        desnaPodlaktica = glm::scale(desnaPodlaktica, glm::vec3(0.18f, 0.5f, 0.22f));
        drawComponent(_desnaRukaDole, desnaPodlaktica);
    }

    bool onKeyPressed(const gui::Key& key) override
    {
        auto c = key.getChar();
        if (c == 'w') _kretanjeNaprijed = true;
        if (c == 's') _kretanjeNazad = true;
        if (c == 'a') _rotacijaLijevo = true;
        if (c == 'd') _rotacijaDesno = true;
        return true;
    }

    bool onKeyReleased(const gui::Key& key) override
    {
        auto c = key.getChar();
        if (c == 'w') _kretanjeNaprijed = false;
        if (c == 's') _kretanjeNazad = false;
        if (c == 'a') _rotacijaLijevo = false;
        if (c == 'd') _rotacijaDesno = false;
        return true;
    }

public:
    MainView()
    {
        gui::gl::Context reqContext(gui::gl::Context::Animation::Yes, gui::gl::DepthBuffer::Size::B3); // Koristimo B24 kao sigurnu opciju
        createContext(reqContext, { gui::InputDevice::Event::Keyboard });
    }

    ~MainView() {
        // TODO: Dodati kod za brisanje OpenGL objekata
    }
};