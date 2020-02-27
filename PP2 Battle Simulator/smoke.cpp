#include "smoke.h"
#include "precomp.h"
namespace Tmpl8
{

void Smoke::Tick()
{
    if (++current_frame == 60) current_frame = 0;
}

void Smoke::Draw(Surface* screen)
{
    smoke_sprite.SetFrame(current_frame / 15);

    smoke_sprite.Draw(screen, (int)position.x, (int)position.y);
}

} // namespace Tmpl8