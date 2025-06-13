#include "clip/clip.h"
#include "common/common.h"
#include "sapper/sapper.h"

int
main(const int argc, char **argv)
{
        // результат или выход
        const struct command *command = clip(argc, argv);
        // результат или выход
        struct tnt **         tnt     = find_tnt(command);
        apply_tnt(tnt);
        return 0;
}
