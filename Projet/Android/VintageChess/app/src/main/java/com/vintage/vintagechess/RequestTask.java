package com.vintage.vintagechess;

import android.content.res.Resources;
import android.os.AsyncTask;
import android.util.Log;

import org.json.JSONException;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.Reader;
import java.io.UnsupportedEncodingException;
import java.net.HttpURLConnection;
import java.net.URL;

/**
 * Created by User on 2016-11-03.
 */

class RequestTask extends AsyncTask<String, String, String> {

    private RequestCallback callback;
    public RequestTask(RequestCallback callback) {
        super();
        this.callback = callback;
    }
    @Override
    protected String doInBackground(String... args) {
        
        // params comes from the execute() call: params[0] is the url.
       try {
            return downloadUrl("http://132.207.89." + args[0], args[1], args[2]);//urls[0]);
        } catch (Exception e) {
            e.printStackTrace();
            return "Unable to retrieve data";
        }
    }
    // onPostExecute displays the results of the AsyncTask.
    @Override
    protected void onPostExecute(String result) {
        //appeler ce qui doit etre fait avec le resultat
        //Utilities.messageBox("Get result", );

        try {
            callback.runResponse(result);
        } catch (JSONException e) {
            e.printStackTrace();
        }

    }


    private String downloadUrl(String myurl, String method, String body) throws Exception {
        InputStream is = null;
        // Only display the first 500 characters of the retrieved
        // web page content.
        //int len = 500;

        try {
            URL url = new URL(myurl);
            HttpURLConnection conn = (HttpURLConnection) url.openConnection();
            conn.setReadTimeout(5000 /* milliseconds */);
            conn.setConnectTimeout(7000 /* milliseconds */);
            conn.setDoInput(true);
            //conn.setDoOutput(true);
            conn.setRequestMethod(method);
            if (!body.equals("")) {

                conn.setRequestProperty("Content-Type", "application/json");
                conn.setRequestProperty("Accept", "application/json");
                OutputStreamWriter wr = new OutputStreamWriter(conn.getOutputStream());
                wr.write("helloworld" + "\r\n\r\n");
                wr.write(body);
                wr.flush();
            }
            else {
                OutputStreamWriter wr = new OutputStreamWriter(conn.getOutputStream());
                wr.write("helloworld" + "\r\n\r\n");
                wr.flush();
            }


            //conn.setRequestProperty("content-length", );
            // Starts the query
            conn.connect();

            int responseCode = conn.getResponseCode();
            is = conn.getInputStream();
            int len = conn.getContentLength();
            String responseMessage = conn.getResponseMessage();
            Log.d("content length", ""+len);
            Log.d("Response message",  responseMessage);
            String contentAsString;
            if (responseCode != 200) throw new Exception(responseCode + " : " + responseMessage);
            if (len > 0) {
                // Convert the InputStream into a string
                contentAsString = readIt(is, len);

                Log.d("HTTP GET", "The response is: " + contentAsString);
            }
            conn.disconnect();
            contentAsString = "";
            return contentAsString;

            // Makes sure that the InputStream is closed after the app is
            // finished using it.
        } finally {
            if (is != null) {
                is.close();
            }
        }
    }


    // Reads an InputStream and converts it to a String.
    public String readIt(InputStream stream, int len) throws IOException, UnsupportedEncodingException {
        Reader reader = null;
        reader = new InputStreamReader(stream, "UTF-8");
        char[] buffer = new char[len];
        reader.read(buffer);
        return new String(buffer);
    }
}