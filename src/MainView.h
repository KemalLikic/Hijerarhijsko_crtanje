#pragma once
#include <gui/gl/View.h>
#include <gui/Key.h>
#include "ShapeGenerator.h"
#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

// ... struct ModelComponent ...
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
    Camera _kamera;

    // ... Komponente za humanoida ...
    ModelComponent _trup, _glava, _lijevaNogaGore, _lijevaNogaDole, _desnaNogaGore, _desnaNogaDole,
        _lijevaRukaGore, _lijevaRukaDole, _desnaRukaGore, _desnaRukaDole;
    ModelComponent _lijevoRame, _desnoRame, _lijeviLakat, _desniLakat,
        _lijeviKuk, _desniKuk, _lijevoKoljeno, _desnoKoljeno;

    // ... Varijable za animaciju i kretanje lika ...
    float _ugaoHoda = 0.0f;
    float _brzinaHoda = 4.0f;
    bool _kretanjeNaprijed = false, _kretanjeNazad = false, _rotacijaLijevo = false, _rotacijaDesno = false;
    float _humanoidPozicijaX = 0.0f, _humanoidPozicijaZ = 0.0f;
    float _humanoidUgaoRotacije = glm::pi<float>();
    float _humanoidBrzinaKretanja = 0.1f, _humanoidBrzinaRotacije = 0.05f;

    // Varijable za kameru
    bool _kameraZoomIn = false;
    bool _kameraZoomOut = false;
    float _kameraUdaljenost = 15.0f; // Malo veća početna udaljenost
    float _kameraBrzinaZuma = 0.3f;
    // Vraćamo ugao za rotaciju kamere, ali ga nećemo vezati za QE
    float _kameraUgaoOkoLika = 0.0f;

    bool _mahanjeAktivno = false;
    float _ugaoMahanja = 0.0f;
private:
    // ... setupShaders, createModelComponent, drawComponent, setup ...
    bool setupShaders()
    {
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
        const glm::vec4 bojaTrupa(0.0f, 0.0f, 1.0f, 1.0f);
        const glm::vec4 bojaGlave(1.0f, 0.8f, 0.6f, 1.0f);
        const glm::vec4 bojaUdova(1.0f, 0.0f, 0.0f, 1.0f);
        const glm::vec4 bojaZglobova(1.0f, 1.0f, 0.0f, 1.0f);
        const glm::vec4 Crna(0.0f, 0.0f, 0.0f, 1.0f);
        ShapeData valjakData = ShapeGenerator::createCylinder(1.0f, 0.5f, 16);
        ShapeData kockaData = ShapeGenerator::createCube();
        ShapeData sferaData = ShapeGenerator::createSphere(1.0f, 24, 12);
        ShapeData loptaData = ShapeGenerator::createSphere(1.0f, 32, 16);
        createModelComponent(valjakData, bojaTrupa, _trup);
        createModelComponent(loptaData, bojaGlave, _glava);
        createModelComponent(kockaData, Crna, _lijevaNogaGore);
        createModelComponent(kockaData, Crna, _lijevaNogaDole);
        createModelComponent(kockaData, Crna, _desnaNogaGore);
        createModelComponent(kockaData, Crna, _desnaNogaDole);
        createModelComponent(kockaData, bojaTrupa, _lijevaRukaGore);
        createModelComponent(kockaData, bojaGlave, _lijevaRukaDole);
        createModelComponent(kockaData, bojaTrupa, _desnaRukaGore);
        createModelComponent(kockaData, bojaGlave, _desnaRukaDole);
        createModelComponent(sferaData, bojaTrupa, _lijevoRame);
        createModelComponent(sferaData, bojaTrupa, _desnoRame);
        createModelComponent(sferaData, bojaGlave, _lijeviLakat);
        createModelComponent(sferaData, bojaGlave, _desniLakat);
        createModelComponent(sferaData, Crna, _lijeviKuk);
        createModelComponent(sferaData, Crna, _desniKuk);
        createModelComponent(sferaData, Crna, _lijevoKoljeno);
        createModelComponent(sferaData, Crna, _desnoKoljeno);
        _matrixID = glGetUniformLocation(_mainPassProgramID, "MVP");
        _objectColorID = glGetUniformLocation(_mainPassProgramID, "objectColor");
    }

protected:
    void onInit() override
    {
        if (!setupShaders()) { assert(false); return; }
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        setup();
        setFocus();
    }

    void onResize(const gui::Size& newSize) override
    {
        View::onResize(newSize);
        _kamera.setViewportSize(newSize);
    }

    bool prepareNextFrame() override
    {
        if (_kretanjeNaprijed || _kretanjeNazad) {
            _ugaoHoda += 0.05f * _brzinaHoda;
        }
        else {
            float ciljniUgao = round(_ugaoHoda / glm::pi<float>()) * glm::pi<float>();
            _ugaoHoda += (ciljniUgao - _ugaoHoda) * 0.1f;
        }

        // Ažuriranje zumiranja kamere
        if (_kameraZoomIn) _kameraUdaljenost -= _kameraBrzinaZuma;
        if (_kameraZoomOut) _kameraUdaljenost += _kameraBrzinaZuma;
        if (_kameraUdaljenost < 3.0f) _kameraUdaljenost = 3.0f;
        if (_kameraUdaljenost > 50.0f) _kameraUdaljenost = 50.0f;

        // Ažuriranje rotacije lika
        if (_rotacijaLijevo) _humanoidUgaoRotacije += _humanoidBrzinaRotacije;
        if (_rotacijaDesno) _humanoidUgaoRotacije -= _humanoidBrzinaRotacije;

        // Ažuriranje pozicije lika
        float moveStep = 0.0f;
        if (_kretanjeNaprijed) moveStep += _humanoidBrzinaKretanja;
        if (_kretanjeNazad) moveStep -= _humanoidBrzinaKretanja;
        _humanoidPozicijaX += moveStep * -glm::cos(_humanoidUgaoRotacije);
        _humanoidPozicijaZ += moveStep * glm::sin(_humanoidUgaoRotacije);

        return true;
    }

    void onDraw(const gui::Rect& rect) override
    {
        // 1. AŽURIRANJE MATRICA
        glm::vec3 targetPozicija = glm::vec3(_humanoidPozicijaX, 1.5f, _humanoidPozicijaZ);
        float visinaKamere = 5.0f;

        // ISPRAVAK: Kamera se pozicionira na osnovu svog ugla, ne ugla lika
        float camX = targetPozicija.x + _kameraUdaljenost * cos(_kameraUgaoOkoLika);
        float camZ = targetPozicija.z + _kameraUdaljenost * sin(_kameraUgaoOkoLika);
        glm::vec3 cameraPosition = glm::vec3(camX, visinaKamere, camZ);

        _viewMatrix = glm::lookAt(cameraPosition, targetPozicija, glm::vec3(0, 1, 0));
        _perspectiveMatrix = _kamera.getPerspectiveMatrix();

        // 2. CRTANJE SCENE
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(_mainPassProgramID);

        // ... ostatak onDraw (animacija, crtanje) je nepromijenjen ...
        float animUgao = _ugaoHoda;
        float ugaoHoda = sin(animUgao) * 0.5f;
        float ugaoKoljena = glm::max(0.0f, -sin(animUgao) * 0.8f);
        float ugaoKoljenaSuprotno = glm::max(0.0f, sin(animUgao) * 0.8f);
        glm::mat4 humanoidMatrix = glm::mat4(1.0f);
        humanoidMatrix = glm::translate(humanoidMatrix, glm::vec3(_humanoidPozicijaX, 0.0f, _humanoidPozicijaZ));
        humanoidMatrix = glm::rotate(humanoidMatrix, _humanoidUgaoRotacije - glm::pi<float>() / 2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 pozicijaTrupaMatrix = glm::translate(humanoidMatrix, glm::vec3(0.0f, 1.5f, 0.0f));
        drawComponent(_trup, glm::scale(pozicijaTrupaMatrix, glm::vec3(1.0f, 1.7f, 0.75f)));
        drawComponent(_glava, glm::scale(glm::translate(pozicijaTrupaMatrix, glm::vec3(0.0f, 1.1f, 0.0f)), glm::vec3(0.5f)));
        float duzinaNadlaktice = 0.7f;
        float duzinaPodlaktice = 0.7f;
        float duzinaNadkoljenice = 0.9f;
        float duzinaPotkoljenice = 0.8f;
        glm::mat4 lijevoRameMatrix = glm::translate(pozicijaTrupaMatrix, glm::vec3(0.65f, 0.7f, 0.0f));
        lijevoRameMatrix = glm::rotate(lijevoRameMatrix, ugaoHoda, glm::vec3(1.0f, 0.0f, 0.0f));
        drawComponent(_lijevoRame, glm::scale(lijevoRameMatrix, glm::vec3(0.18f)));
        glm::mat4 lijevaNadlakticaMatrix = glm::translate(lijevoRameMatrix, glm::vec3(0.0f, -duzinaNadlaktice / 2.0f, 0.0f));
        drawComponent(_lijevaRukaGore, glm::scale(lijevaNadlakticaMatrix, glm::vec3(0.2f, duzinaNadlaktice, 0.25f)));
        glm::mat4 lijeviLakatMatrix = glm::translate(lijevoRameMatrix, glm::vec3(0.0f, -duzinaNadlaktice, 0.0f));
        lijeviLakatMatrix = glm::rotate(lijeviLakatMatrix, -ugaoKoljena, glm::vec3(1.0f, 0.0f, 0.0f));
        drawComponent(_lijeviLakat, glm::scale(lijeviLakatMatrix, glm::vec3(0.14f)));
        glm::mat4 lijevaPodlakticaMatrix = glm::translate(lijeviLakatMatrix, glm::vec3(0.0f, -duzinaPodlaktice / 2.0f, 0.0f));
        drawComponent(_lijevaRukaDole, glm::scale(lijevaPodlakticaMatrix, glm::vec3(0.18f, duzinaPodlaktice, 0.22f)));
        glm::mat4 desnoRameMatrix = glm::translate(pozicijaTrupaMatrix, glm::vec3(-0.65f, 0.7f, 0.0f));
        desnoRameMatrix = glm::rotate(desnoRameMatrix, -ugaoHoda, glm::vec3(1.0f, 0.0f, 0.0f));
        drawComponent(_desnoRame, glm::scale(desnoRameMatrix, glm::vec3(0.18f)));
        glm::mat4 desnaNadlakticaMatrix = glm::translate(desnoRameMatrix, glm::vec3(0.0f, -duzinaNadlaktice / 2.0f, 0.0f));
        drawComponent(_desnaRukaGore, glm::scale(desnaNadlakticaMatrix, glm::vec3(0.2f, duzinaNadlaktice, 0.25f)));
        glm::mat4 desniLakatMatrix = glm::translate(desnoRameMatrix, glm::vec3(0.0f, -duzinaNadlaktice, 0.0f));
        desniLakatMatrix = glm::rotate(desniLakatMatrix, -ugaoKoljenaSuprotno, glm::vec3(1.0f, 0.0f, 0.0f));
        drawComponent(_desniLakat, glm::scale(desniLakatMatrix, glm::vec3(0.14f)));
        glm::mat4 desnaPodlakticaMatrix = glm::translate(desniLakatMatrix, glm::vec3(0.0f, -duzinaPodlaktice / 2.0f, 0.0f));
        drawComponent(_desnaRukaDole, glm::scale(desnaPodlakticaMatrix, glm::vec3(0.18f, duzinaPodlaktice, 0.22f)));
        glm::mat4 lijeviKukMatrix = glm::translate(pozicijaTrupaMatrix, glm::vec3(-0.35f, -0.85f, 0.0f));
        lijeviKukMatrix = glm::rotate(lijeviKukMatrix, ugaoHoda, glm::vec3(1.0f, 0.0f, 0.0f));
        drawComponent(_lijeviKuk, glm::scale(lijeviKukMatrix, glm::vec3(0.20f)));
        glm::mat4 lijevaNadkoljenicaMatrix = glm::translate(lijeviKukMatrix, glm::vec3(0.0f, -duzinaNadkoljenice / 2.0f, 0.0f));
        drawComponent(_lijevaNogaGore, glm::scale(lijevaNadkoljenicaMatrix, glm::vec3(0.3f, duzinaNadkoljenice, 0.35f)));
        glm::mat4 lijevoKoljenoMatrix = glm::translate(lijeviKukMatrix, glm::vec3(0.0f, -duzinaNadkoljenice, 0.0f));
        lijevoKoljenoMatrix = glm::rotate(lijevoKoljenoMatrix, ugaoKoljena, glm::vec3(1.0f, 0.0f, 0.0f));
        drawComponent(_lijevoKoljeno, glm::scale(lijevoKoljenoMatrix, glm::vec3(0.16f)));
        glm::mat4 lijevaPotkoljenicaMatrix = glm::translate(lijevoKoljenoMatrix, glm::vec3(0.0f, -duzinaPotkoljenice / 2.0f, 0.0f));
        drawComponent(_lijevaNogaDole, glm::scale(lijevaPotkoljenicaMatrix, glm::vec3(0.25f, duzinaPotkoljenice, 0.3f)));
        glm::mat4 desniKukMatrix = glm::translate(pozicijaTrupaMatrix, glm::vec3(0.35f, -0.85f, 0.0f));
        desniKukMatrix = glm::rotate(desniKukMatrix, -ugaoHoda, glm::vec3(1.0f, 0.0f, 0.0f));
        drawComponent(_desniKuk, glm::scale(desniKukMatrix, glm::vec3(0.20f)));
        glm::mat4 desnaNadkoljenicaMatrix = glm::translate(desniKukMatrix, glm::vec3(0.0f, -duzinaNadkoljenice / 2.0f, 0.0f));
        drawComponent(_desnaNogaGore, glm::scale(desnaNadkoljenicaMatrix, glm::vec3(0.3f, duzinaNadkoljenice, 0.35f)));
        glm::mat4 desnoKoljenoMatrix = glm::translate(desniKukMatrix, glm::vec3(0.0f, -duzinaNadkoljenice, 0.0f));
        desnoKoljenoMatrix = glm::rotate(desnoKoljenoMatrix, ugaoKoljenaSuprotno, glm::vec3(1.0f, 0.0f, 0.0f));
        drawComponent(_desnoKoljeno, glm::scale(desnoKoljenoMatrix, glm::vec3(0.16f)));
        glm::mat4 desnaPotkoljenicaMatrix = glm::translate(desnoKoljenoMatrix, glm::vec3(0.0f, -duzinaPotkoljenice / 2.0f, 0.0f));
        drawComponent(_desnaNogaDole, glm::scale(desnaPotkoljenicaMatrix, glm::vec3(0.25f, duzinaPotkoljenice, 0.3f)));
    }

    bool onKeyPressed(const gui::Key& key) override
    {
        auto c = key.getChar();
        if (c == 'w') _kretanjeNaprijed = true;
        if (c == 's') _kretanjeNazad = true;
        if (c == 'a') _rotacijaLijevo = true;
        if (c == 'd') _rotacijaDesno = true;
        if (c == 'q') _kameraZoomOut = true;
        if (c == 'e') _kameraZoomIn = true;
        return true;
    }

    bool onKeyReleased(const gui::Key& key) override
    {
        auto c = key.getChar();
        if (c == 'w') _kretanjeNaprijed = false;
        if (c == 's') _kretanjeNazad = false;
        if (c == 'a') _rotacijaLijevo = false;
        if (c == 'd') _rotacijaDesno = false;
        if (c == 'q') _kameraZoomOut = false;
        if (c == 'e') _kameraZoomIn = false;
        return true;
    }

public:
    MainView()
        : _kamera(gui::Size(1280, 720))
    {
        gui::gl::Context reqContext(gui::gl::Context::Animation::Yes, gui::gl::DepthBuffer::Size::B3);
        createContext(reqContext, { gui::InputDevice::Event::Keyboard });
    }
    ~MainView() {}
};