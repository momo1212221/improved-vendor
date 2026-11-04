#include <cstdint>
#include <string>
#include "openrac/mod.hpp"
#include "openrac/games/rac1.hpp"
#include "logic.hpp"

namespace {
openrac::Mod* g=nullptr;
bool vendor_opened=false;
int acquired_item=-1;
bool show_wallet=true, purchase_notice=true;
}
void load(openrac::Mod& mod){
    if(mod.game()!="rac1" || !mod.has_world()) throw openrac::Error("Improved Vendor requires RAC1 interface 1.9+");
    g=&mod;
    show_wallet=mod.setting<bool>("show_wallet");
    purchase_notice=mod.setting<bool>("purchase_notice");
    mod.hook<openrac::rac1::Vendor_BuildItemList>([](auto original,std::int32_t skip){
        original(skip); vendor_opened=true;
    });
    mod.hook<openrac::rac1::Item_Give>([](auto original,std::int32_t id,std::int32_t equip){
        original(id,equip);
        const auto s=g->play_status();
        if(s.phase==OPENRAC_PLAY_MENU) acquired_item=id;
    });
    mod.on_frame([]{
        if(vendor_opened){
            vendor_opened=false;
            if(show_wallet && g->has_notice()) g->play_notice(qol::wallet(g->bolts()));
        }
        if(acquired_item>=0){
            const int id=acquired_item; acquired_item=-1;
            if(purchase_notice && g->has_notice()) g->play_notice(qol::acquired(id,g->bolts()));
        }
    });
    mod.info("vendor wallet and acquisition feedback");
}
OPENRAC_MOD_CPP("improved-vendor","0.1.0",load)
