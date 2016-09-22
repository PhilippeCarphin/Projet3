package inf3995.tp4;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import com.google.api.client.http.HttpRequest;

import java.io.InputStream;

import okhttp3.Response;

/**
 * Created by User on 2016-09-20.
 */
public class RequestBuilder {
    private static Response runRequest(int testNum) {

        try {
            RequeteHttp process = new RequeteHttp();
            process.execute(Integer.valueOf(testNum).toString(), "192.168.0.112");
            return process.get();
        }
        catch (Exception exc) {
            exc.printStackTrace();
        }
        return null;
    }


    private static void test1() {
        try {
            Response r = runRequest(1);
            String s = r.body().string();
            //ecrire dans le view
        }
        catch(Exception ex) {
            ex.printStackTrace();
        }
    }

    private static void test2() {
        try {
            Response r = runRequest(2);
            String s = r.body().string();
            //convertir en html et ecrire dans le view
        }
        catch(Exception ex) {
            ex.printStackTrace();
        }
    }

    private static void test3() {
        try {
            Response r = runRequest(3);
            InputStream inputStream = r.body().byteStream();
            Bitmap bitmap = BitmapFactory.decodeStream(inputStream);
            //mettre le bitmap dans le view
        }
        catch(Exception ex) {
            ex.printStackTrace();
        }
    }

    public void gererTestNum(int testNum) {
        switch(testNum) {
            case 1:
                test1();
                break;
            case 2:
                test2();
                break;
            case 3:
                test3();
                break;
            default:
                break;
        }
    }

}

