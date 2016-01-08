using Microsoft.Kinect;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace KinectSilhouetteDriver
{

    /// <summary>
    /// Логика взаимодействия для MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {

        KinectSensor Kinect;

        public MainWindow()
        {
            InitializeComponent();
            Kinect = KinectSensor.KinectSensors[0];
            Kinect.AllFramesReady += Kinect_AllFramesReady;
            Kinect.DepthStream.Enable(DepthImageFormat.Resolution320x240Fps30);
            Kinect.SkeletonStream.Enable();
            Kinect.Start();
        }

        protected DepthImagePixel get(DepthImagePixel[] A, int i, int j)
        {
            return A[j * 320 + i];
        }

        int cnt = 0;

        private void Kinect_AllFramesReady(object sender, AllFramesReadyEventArgs e)
        {
            if (cnt++ < 15) return;
            cnt = 0;
            using (var f = e.OpenDepthImageFrame())
            {
                var pd = f.GetRawPixelData();
                int mx = 1000, my = 1000, Mx = 0, My = 0;
                for (int i = 0; i < 320; i++)
                {
                    for (int j = 0; j < 240; j++)
                    {
                        if (get(pd, i, j).PlayerIndex > 0)
                        {
                            if (i < mx) mx = i;
                            if (i > Mx) Mx = i;
                            if (j < my) my = j;
                            if (j > My) My = j;
                        }
                    }
                }
                if (mx<1000)
                {
                    float wc = (Mx - mx) / 16;
                    float hc = (My - my) / 16;
                    Console.WriteLine("mx={0},Mx={1},my={2},My={3}", mx, Mx, my, My);
                    for (int j=0;j<16;j++)
                    {
                        for (int i=0;i<16;i++)
                        {
                            if (get(pd, mx + (int)(wc * i), my + (int)(hc * j)).PlayerIndex > 0)
                            {
                                Console.Write("*");
                            }
                            else Console.Write(" ");
                        }
                        Console.WriteLine();
                    }
                }
                // Console.Write("{0}\r", pd.Length);
            }
        }
    }
}
