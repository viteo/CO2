using Caliburn.Micro;
using System.Windows;
using TinyCO2mvvm.ViewModels;

namespace TinyCO2mvvm
{
    public class AppBootstrapper : BootstrapperBase
    {
        public AppBootstrapper()
        {
            Initialize();
        }
        protected override void OnStartup(object sender, StartupEventArgs e)
        {
            DisplayRootViewFor<ShellViewModel>();
        }
    }
}
