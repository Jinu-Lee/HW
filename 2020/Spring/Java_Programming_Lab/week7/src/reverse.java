import java.io.*;
import java.util.ArrayList;

public class reverse {
    public static void main(String[] args) throws IOException, FileNotFoundException{  
        InputStream fis = new FileInputStream("input.txt");
        InputStreamReader isr = new InputStreamReader(fis);
        BufferedReader br = new BufferedReader(isr);

        OutputStream fos = new FileOutputStream("output.txt");
        OutputStreamWriter osw = new OutputStreamWriter(fos);
        BufferedWriter bw = new BufferedWriter(osw);

        ArrayList<String> arr = new ArrayList<>();
        String temp = new String();
        while((temp = br.readLine()) != null) {
            arr.add(temp);
        }

        for(int i = arr.size() - 1; i >= 0; i--) {
            bw.write(arr.get(i));
            if (i != 0)
                bw.newLine();
        }
        bw.flush();

        fis.close(); isr.close(); br.close();
        fos.close(); osw.close(); bw.close();
    }
}