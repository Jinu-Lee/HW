import java.io.DataOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.net.Socket;
import java.nio.file.Files;
import java.nio.file.Paths;

public class CommThread extends Thread {
	private Socket soc;
	private int id;
	
	public CommThread (Socket soc, int id) {
		this.soc = soc;
		this.id = id;
	}
	
	public void run() {
		try {
			OutputStream os = soc.getOutputStream();
			DataOutputStream dos = new DataOutputStream (os);
			byte[] file = Files.readAllBytes(Paths.get("test.mp4"));
			
			dos.writeInt(id);
			dos.writeInt(file.length);
			dos.write(file, 0, file.length);
			
			dos.writeUTF("message from server (" + id + ")");
			System.out.println(Server.getLog("message is sent (" + id + ")"));
			
			dos.close();
			this.soc.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
