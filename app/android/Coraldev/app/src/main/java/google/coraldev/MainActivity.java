package google.coraldev;

import android.Manifest;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothManager;
import android.bluetooth.le.BluetoothLeScanner;
import android.bluetooth.le.ScanCallback;
import android.bluetooth.le.ScanResult;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Handler;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;

import java.util.ArrayList;
import java.util.List;

public class MainActivity extends Activity {
    private static final String TAG = "CoralMain";
    private BluetoothManager mBtManager;
    private BluetoothAdapter mBtAdapter;
    private BluetoothAdapter.LeScanCallback mBtLeScanCallback;

    // Members specifically used for BLE scanning
    private Boolean mHaveLocationPermissions = false;
    private Boolean mIsScanning = false;
    private ArrayAdapter<String> mScanResultListAdapter;
    private Button mScanButton;
    private ListView scanResultList;
    private ArrayList<String> mScanResults;
    private Handler mHandler;

    private final static int REQUEST_ENABLE_BT = 1;
    private final static int REQUEST_LOCATION_ACCESS = 2;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.d(TAG, "onCreate");
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mBtManager = (BluetoothManager) getSystemService(Context.BLUETOOTH_SERVICE);
        mBtAdapter = mBtManager.getAdapter();
        mScanResults = new ArrayList<String>();

        mHandler = new Handler();
        mScanResultListAdapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1);
        scanResultList = (ListView) findViewById(R.id.scanResultList);
        scanResultList.setAdapter(mScanResultListAdapter);

        mScanButton = (Button) findViewById(R.id.btnScan);
    }

    @Override
    protected void onStart() {
        Log.d(TAG, "onStart");
        super.onStart();


    }

    @Override
    protected void onResume() {
        Log.d(TAG, "onResume");
        super.onResume();
    }

    @Override
    protected void onPause() {
        Log.d(TAG, "onPause");
        super.onPause();
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        switch (requestCode) {
            case REQUEST_ENABLE_BT:
                Log.d(TAG, (resultCode == RESULT_OK) ? "BT Enabled\n" : "BT still Disabled\n");
                break;
            default:
                Log.d(TAG, "Unhandled ActivityResult from request "
                        + requestCode + ": " + resultCode);
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode,
                                           String permissions[], int[] grantResults) {
        switch (requestCode) {
            case REQUEST_LOCATION_ACCESS: {
                // If request is cancelled, the result arrays are empty.
                if (grantResults.length > 0) {
                    mHaveLocationPermissions =
                            (grantResults[0] == PackageManager.PERMISSION_GRANTED);
                    return;
                }
            }

            default: {
                Log.d(TAG, "Unhandled PermissionsReqsult from request " + requestCode);
            }
        }
    }

    public class onScan extends ScanCallback {
        @Override
        public void onScanFailed(int errorCode) {
            Log.d(TAG, "onScanFailed\n");
        }

        @Override
        public void onScanResult(int callbackType, ScanResult result) {
            Log.d(TAG, result.toString() + "\n");

            BluetoothDevice dev = result.getDevice();
            String device = dev.getName() + " " + dev.getAddress();

            if (!mScanResults.contains(device)) {
                mScanResults.add(device);
                mScanResultListAdapter.add(device);
            }
        }

        @Override
        public void onBatchScanResults(List<ScanResult> results) {
            Log.d(TAG, "onBatchScanResult\n");

        }
    }

    public void onLeScan(final BluetoothDevice device, int rssi, byte[] scanRecord) {
        Log.d(TAG, device.getName() + " rssi " + rssi);
    }

    public void scanLeDevices(View view) {
        Log.d(TAG, "Scan pressed!\n");
        if (mBtAdapter == null || !mBtAdapter.isEnabled()) {
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
            Log.d(TAG, "BT enabled!");
        }

        // Do we have permission for location to do scans?
        mHaveLocationPermissions =
                (checkSelfPermission(Manifest.permission.ACCESS_COARSE_LOCATION)
                        == PackageManager.PERMISSION_GRANTED);

        if (!mHaveLocationPermissions) {
            requestPermissions(new String[]{Manifest.permission.ACCESS_COARSE_LOCATION},
                    REQUEST_LOCATION_ACCESS);
        }

        // Scan once we have appropriate permissions and services enabled.
        if (mHaveLocationPermissions && mBtAdapter.isEnabled()) {
            final ScanCallback mOnScan = new onScan();
            final BluetoothLeScanner mBtLeScanner = mBtAdapter.getBluetoothLeScanner();
            mBtLeScanner.startScan(mOnScan);
            mIsScanning = true;
            mScanResultListAdapter.clear();
            mScanResults.clear();
            mScanButton.setText("scanning");
            mScanButton.setEnabled(false);


            mHandler.postDelayed(new Runnable() {
                @Override
                public void run() {
                    mIsScanning = false;
                    mBtLeScanner.stopScan(mOnScan);
                    mScanButton.setText("scan");
                    mScanButton.setEnabled(true);
                    Log.d(TAG, "Stopping scan\n");
                }
            }, 10000);
        }
    }

}
