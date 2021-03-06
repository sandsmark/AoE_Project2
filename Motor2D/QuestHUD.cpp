#include "QuestHUD.h"
#include "Application.h"
#include "Gui.h"
#include "Window.h"
#include "Fonts.h"

void QuestHUD::Start()
{
    int x, y;
    App->win->GetWindowSize(x, y);
    winx = x;
    winy = y;

    objective_lbl = (Label *)App->gui->CreateLabel("Objectives", x - App->render->camera.x - 300, y / 2 - App->render->camera.y, App->font->fonts[SIXTEEN]);
    objective_lbl->SetColor({ 255, 255, 255, 255 });

    desc_lbl = (Label *)App->gui->CreateLabel("Destroy Sauron's tower to win.", x - App->render->camera.x - 300, y / 2 - App->render->camera.y + 22, nullptr);
    desc_lbl->SetColor({ 255, 25, 25, 255 });

    quest_lbl = (Label *)App->gui->CreateLabel("Side Quests", x - App->render->camera.x - 300, y / 2 - App->render->camera.y + 36, App->font->fonts[SIXTEEN]);
    quest_lbl->SetColor({ 255, 255, 255, 255 });
}

void QuestHUD::Update()
{
    int x, y;
    App->win->GetWindowSize(x, y);
    winx = x;
    winy = y;

    questx = x - App->render->camera.x - 300;
    questy = y / 2 - App->render->camera.y + 58;

    Sprite quad;
    quad.rect.x = winx - App->render->camera.x - 300;
    quad.rect.y = winy / 2 - App->render->camera.y;
    quad.rect.w = 300;
    quad.rect.h = 175;
    quad.a = 180;
    App->render->ui_toDraw.push_back(quad);

    if (vec_quest.size() == 0 && no_quest_lbl == nullptr) {
        no_quest_lbl = (Label *)App->gui->CreateLabel("No quests to complete", questx, questy, nullptr);
        no_quest_lbl->SetColor({ 255, 255, 255, 255 });
    } else if (vec_quest.size() > 0 && no_quest_lbl != nullptr) {
        App->gui->DestroyUIElement(no_quest_lbl);
        no_quest_lbl = nullptr;
    } else if (vec_quest.size() > 0) {
        if (vec_quest.size() == 1) {
            if (vec_quest.front()->name_lbl == nullptr) {
                vec_quest.front()->name += ":";
                vec_quest.front()->name_lbl = (Label *)App->gui->CreateLabel(vec_quest.front()->name, questx, questy, App->font->fonts[FOURTEEN]);
                vec_quest.front()->name_lbl->SetColor({ 255, 165, 0, 255 });
                vec_quest.front()->desc_lbl = (Label *)App->gui->CreateLabel(vec_quest.front()->desc, questx, questy + 14, nullptr);
                vec_quest.front()->desc_lbl->SetColor({ 255, 255, 255, 255 });
            }
        }
        if (vec_quest.size() == 2) {
            if (vec_quest.front()->name_lbl == nullptr) {
                vec_quest.front()->name += ":";
                vec_quest.front()->name_lbl = (Label *)App->gui->CreateLabel(vec_quest.front()->name, questx, questy, App->font->fonts[FOURTEEN]);
                vec_quest.front()->name_lbl->SetColor({ 255, 165, 0, 255 });
                vec_quest.front()->desc_lbl = (Label *)App->gui->CreateLabel(vec_quest.front()->desc, questx, questy + 14, nullptr);
                vec_quest.front()->desc_lbl->SetColor({ 255, 255, 255, 255 });
            } else {
                vec_quest.front()->name_lbl->pos.first = questx;
                vec_quest.front()->name_lbl->pos.second = questy;
                vec_quest.front()->desc_lbl->pos.first = questx;
                vec_quest.front()->desc_lbl->pos.second = questy + 14;
            }
            if (vec_quest.back()->name_lbl == nullptr) {
                vec_quest.back()->name += ":";
                vec_quest.back()->name_lbl = (Label *)App->gui->CreateLabel(vec_quest.back()->name, questx, questy + 14 + 12, App->font->fonts[FOURTEEN]);
                vec_quest.back()->name_lbl->SetColor({ 255, 165, 0, 255 });
                vec_quest.back()->desc_lbl = (Label *)App->gui->CreateLabel(vec_quest.back()->desc, questx, questy + 14 + 12 + 14, nullptr);
                vec_quest.back()->desc_lbl->SetColor({ 255, 255, 255, 255 });
            } else {
                vec_quest.back()->name_lbl->pos.first = questx;
                vec_quest.back()->name_lbl->pos.second = questy + 14 + 12;
                vec_quest.back()->desc_lbl->pos.first = questx;
                vec_quest.back()->desc_lbl->pos.second = questy + 14 + 12 + 14;
            }
        }
        if (vec_quest.size() == 3) {
            if (vec_quest.front()->name_lbl == nullptr) {
                vec_quest.front()->name += ":";
                vec_quest.front()->name_lbl = (Label *)App->gui->CreateLabel(vec_quest.front()->name, questx, questy, App->font->fonts[FOURTEEN]);
                vec_quest.front()->name_lbl->SetColor({ 255, 165, 0, 255 });
                vec_quest.front()->desc_lbl = (Label *)App->gui->CreateLabel(vec_quest.front()->desc, questx, questy + 14, nullptr);
                vec_quest.front()->desc_lbl->SetColor({ 255, 255, 255, 255 });
            } else {
                vec_quest.front()->name_lbl->pos.first = questx;
                vec_quest.front()->name_lbl->pos.second = questy;
                vec_quest.front()->desc_lbl->pos.first = questx;
                vec_quest.front()->desc_lbl->pos.second = questy + 14;
            }
            if (vec_quest.back()->name_lbl == nullptr) {
                vec_quest.back()->name += ":";
                vec_quest.back()->name_lbl = (Label *)App->gui->CreateLabel(vec_quest.back()->name, questx, questy + 14 + 12, App->font->fonts[FOURTEEN]);
                vec_quest.back()->name_lbl->SetColor({ 255, 165, 0, 255 });
                vec_quest.back()->desc_lbl = (Label *)App->gui->CreateLabel(vec_quest.back()->desc, questx, questy + 14 + 12 + 14, nullptr);
                vec_quest.back()->desc_lbl->SetColor({ 255, 255, 255, 255 });
            } else {
                vec_quest.back()->name_lbl->pos.first = questx;
                vec_quest.back()->name_lbl->pos.second = questy + 14 + 12;
                vec_quest.back()->desc_lbl->pos.first = questx;
                vec_quest.back()->desc_lbl->pos.second = questy + 14 + 12 + 14;
            }
        }
    }
}

void QuestHUD::CleanUp()
{
    App->gui->DestroyUIElement(objective_lbl);
    objective_lbl = nullptr;
    App->gui->DestroyUIElement(desc_lbl);
    desc_lbl = nullptr;
    App->gui->DestroyUIElement(quest_lbl);
    quest_lbl = nullptr;
    App->gui->DestroyUIElement(no_quest_lbl);
    no_quest_lbl = nullptr;

    if (vec_quest.size() > 0) {
        int h = vec_quest.front()->id;

        for (list<QuestsShown *>::iterator it = vec_quest.begin(); it != vec_quest.end(); ++it) {
            RemoveQuest(h);
            ++h;
        }
        vec_quest.clear();
    }
}

void QuestHUD::AddActiveQuest(string argname, string argdesc, int id)
{

    vector<QuestsShown *> quests;

    for (list<QuestsShown *>::iterator it = vec_quest.begin(); it != vec_quest.end(); ++it) {

        string name((*it)->name);
        string desc((*it)->desc);
        uint i = (*it)->id;
        RemoveQuest(id);

        QuestsShown *quest;
        quest = new QuestsShown();

        quest->AddQuest(name, desc, i);

        quests.push_back(quest);
    }

    for (uint i = 0; i < quests.size(); ++i) {
        vec_quest.push_back(quests[i]);
    }
    quests.clear();

    QuestsShown *quest;
    quest = new QuestsShown();
    quest->AddQuest(argname, argdesc, id);

    vec_quest.push_back(quest);
    int a = 3;
}

void QuestHUD::RemoveQuest(int argid)
{
    QuestsShown *c;

    for (list<QuestsShown *>::iterator it = vec_quest.begin(); it != vec_quest.end(); ++it) {
        if (argid == (*it)->id) {

            c = (*it);
            (*it)->CleanUpQuest();
            it = vec_quest.erase(it);
        }
    }
}

void QuestsShown::AddQuest(string argname, string argdesc, int argid)
{
    name = argname;
    desc = argdesc;
    id = argid;
}

void QuestsShown::CleanUpQuest()
{
    App->gui->DestroyUIElement(name_lbl);
    name_lbl = nullptr;
    App->gui->DestroyUIElement(desc_lbl);
    desc_lbl = nullptr;
}
