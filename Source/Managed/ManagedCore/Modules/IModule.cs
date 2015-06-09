using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Vanguard.ManagedCore.Modules
{
    public interface IModule
    {
        void LoadModule();
        void UnloadModule();

        String GetModuleName();
        String GetModuleType();
    }
}
