// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "XmppPrivatePCH.h"
#include "XmppNull.h"

class FXmppConnectionNull
	: public IXmppConnection
{
public:

	// IXmppConnection

	virtual void SetServer(const FXmppServer& Server) override
	{}
	virtual const FXmppServer& GetServer() const override
	{ 
		return ServerConfig; 
	}
	virtual void Login(const FString& UserId, const FString& Auth) override
	{
		UserJid.Id = UserId;
		OnLoginComplete().Broadcast(UserJid, false, TEXT("not implemented"));
	}
	virtual void Logout() override
	{
		OnLogoutComplete().Broadcast(UserJid, false, TEXT("not implemented"));
	}
	virtual EXmppLoginStatus::Type GetLoginStatus() const override
	{ 
		return EXmppLoginStatus::LoggedOut; 
	}
	virtual const FXmppUserJid& GetUserJid() const override
	{
		return UserJid;
	}
	virtual FOnXmppLoginComplete& OnLoginComplete() override { return OnXmppLoginCompleteDelegate; }
	virtual FOnXmppLogingChanged& OnLoginChanged() override { return OnXmppLogingChangedDelegate; }
	virtual FOnXmppLogoutComplete& OnLogoutComplete() override { return OnXmppLogoutCompleteDelegate; }

	virtual TSharedPtr<class IXmppPresence> Presence() override { return NULL; }
	virtual TSharedPtr<class IXmppPubSub> PubSub() override { return NULL; }
	virtual TSharedPtr<class IXmppMessages> Messages() override { return NULL; }
	virtual TSharedPtr<class IXmppMultiUserChat> MultiUserChat() override { return NULL; }
	virtual TSharedPtr<class IXmppChat> PrivateChat() override { return NULL; }

	// FXmppConnectionNull

	FXmppConnectionNull() {};
	virtual ~FXmppConnectionNull() {};

private:

	FXmppServer ServerConfig;
	FXmppUserJid UserJid;

	// completion delegates
	FOnXmppLoginComplete OnXmppLoginCompleteDelegate;
	FOnXmppLogingChanged OnXmppLogingChangedDelegate;
	FOnXmppLogoutComplete OnXmppLogoutCompleteDelegate;	
};


TSharedRef<IXmppConnection> FXmppNull::CreateConnection()
{
	UE_LOG(LogXmpp, Warning, TEXT("Xmpp not implemented. Creating FXmppNull connection"));

	return MakeShareable(new FXmppConnectionNull());
}
