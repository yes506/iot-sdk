package com.example.myapplication;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import android.util.Log;


public class MainActivity extends AppCompatActivity {

	private static final String TAG = "MainActivity";


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

		setContentView(R.layout.activity_main);

		//HelloThread h = new HelloThread();
		Hello h = new Hello();

		/*
		String ec_ip   = "dev-gw-iotmakers.cloud.kt.com";
		int    ec_port = 30105;
		String dev_id  = "testbyw111";
		String dev_pw  = "12312";
		String svc_gw  = "KT_3RD_BYW";
		*/

		String ec_ip   = "dev-gw-iotmakers.cloud.kt.com";
		int    ec_port = 30105;
		String dev_id  = "testbyw111";
		String dev_pw  = "12312";
		String dev_gw  = "KT_3RD_BYW";

        h.run_main( ec_ip, ec_port, dev_id, dev_pw, dev_gw );

    }


}