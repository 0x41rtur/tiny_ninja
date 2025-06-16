#include "clip/clip.h"
#include "common/common.h"
#include "executer/executer.h"

int
main(const int argc, char **argv)
{
        const struct command **command = clip(argc, argv);
        const struct command **cmd = command;
        while (NULL != *cmd)
        {
                struct tnt **tnt = find_tnt(*cmd);

                if (NULL != tnt && -1 == execute(tnt))
                {
                        break;
                }
                ++cmd;
        }
        // free
        return 0;
}
