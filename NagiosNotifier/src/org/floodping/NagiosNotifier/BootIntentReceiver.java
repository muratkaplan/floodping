package org.floodping.NagiosNotifier;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

public class BootIntentReceiver extends BroadcastReceiver {

	@Override
	public void onReceive(Context context, Intent intent) {
		Helper.Register(context);
	}
}
