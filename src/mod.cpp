#include <cstdint>
#include <string>
#include "openrac/mod.hpp"
#include "openrac/games/rac1.hpp"
#include "logic.hpp"

namespace {
openrac::Mod* g=nullptr;
openrac::UiPage page;
int last_item=-1;
std::uint16_t previous=0;
bool open_requested=false;
}

void load(openrac::Mod& mod){
    if(mod.game()!="rac1" || !mod.has_world()) throw openrac::Error("Improved Vendor requires RAC1 interface 1.9+");
    g=&mod;

    page=mod.ui_page([&mod](const openrac::UiPage& p,const openrac::UiEvent&){
        openrac::UiContent c;
        c.title="Vendor Info";
        c.items={
            openrac::UiItem::text("Wallet: "+std::to_string(mod.bolts())+" bolts"),
            openrac::UiItem::text(last_item>=0 ? "Last acquired item: "+std::to_string(last_item) : "Last acquired item: -")
        };
        c.footer={"L2 + R2 + Square opens this page"};
        p.set(c);
    });

    mod.hook<openrac::rac1::Vendor_BuildItemList>([](auto original,std::int32_t skip){
        original(skip);
    });
    mod.hook<openrac::rac1::Item_Give>([](auto original,std::int32_t id,std::int32_t equip){
        original(id,equip);
        if(g->play_status().phase==OPENRAC_PLAY_MENU) last_item=id;
    });

    mod.on_pad([](int port,std::uint8_t* report,int size){
        if(port!=0 || report==nullptr || size<2) return;
        const auto now=qol::pad_buttons(report);
        const auto mask=static_cast<std::uint16_t>(OPENRAC_PAD_L2|OPENRAC_PAD_R2|OPENRAC_PAD_SQUARE);
        if(qol::combo_pressed(now,previous,mask)) open_requested=true;
        if((now&mask)==mask) qol::consume_buttons(report,mask);
        previous=now;
    });
    mod.on_frame([]{
        if(open_requested){ open_requested=false; page.open(); }
    });
}
OPENRAC_MOD_CPP("improved-vendor","0.2.0",load)
