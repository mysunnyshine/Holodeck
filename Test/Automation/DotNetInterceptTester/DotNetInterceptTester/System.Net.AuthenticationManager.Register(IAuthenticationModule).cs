namespace DotNetInterceptTester.My_System.Net.AuthenticationManager
{
public class Register_System_Net_IAuthenticationModule
{
public static bool _Register_System_Net_IAuthenticationModule( )
{
   //Parameters
   System.Net.IAuthenticationModule authenticationModule = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.AuthenticationManager.Register(authenticationModule);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.AuthenticationManager.Register(authenticationModule);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
