namespace DotNetInterceptTester.My_System.IO.StreamWriter
{
public class ctor_System_IO_StreamWriter_System_IO_Stream
{
public static bool _ctor_System_IO_StreamWriter_System_IO_Stream( )
{
   //Parameters
   System.IO.Stream stream = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.StreamWriter.ctor(stream);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.StreamWriter.ctor(stream);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}