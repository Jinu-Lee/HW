import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.Iterator;

public class Server {
	private static ArrayList<Thread> arr = new ArrayList<Thread> ();
	private static SimpleDateFormat sdfDate = new SimpleDateFormat("yyy-MM-dd HH:mm:SSS");
	
	public static String getLog (String msg) {
		return "[" + sdfDate.format(new Date()) + "] Server thread: " + msg;
	}
	
	@SuppressWarnings("resource")
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		ServerSocket ss = null;
		int id = 0;
		try {
			ss = new ServerSocket (5000);
		} catch (IOException e) {
			e.printStackTrace();
		}
		System.out.println("server is ready");
		while (true) {
			try {
				Socket soc = ss.accept();
				System.out.println(Server.getLog("new connection arrived"));
				Thread t = new CommThread(soc, id++);
				t.start();
				arr.add(t);
				Iterator<Thread> iter = arr.iterator();
				while (iter.hasNext()) {
					t = iter.next();
					if (!t.isAlive()) {
						iter.remove();
					}
				}
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
}
