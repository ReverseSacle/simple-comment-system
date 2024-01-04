async function _PostRequest(host,url,data)
{
	try{
		const _URL = "http://" + host + url;
		var response = await fetch(
			_URL,
			{
				method: "POST",
				headers:{
					"Content-Type": "text/plain",
				},
				body: data,
			}
		);
		return response.ok;
	} catch(error) { return false; }
}

async function _GetRequest(host,url)
{
	try{
		const _URL = "http://" + host + url;
		var response = await fetch(_URL);

		if(!response.ok){ return [false,""]; }

		return [true,await response.text()];
	} catch(error) { return [false,""]; }
}

function textareaBeautify(textsLable,cols)
{
	var cache = "";
	var texts = (textsLable.value).replace(/\r|\n/g,"");
	var textLength = texts.length;

	for(var i = 0;i < textLength;++i)
	{
		if(0 != cache.length && 0 === cache.length % cols){
			cache += "<br/>";
		}
		cache += texts[i];
	};
	return cache;
}

function avatarAppend(main_label,email_md5)
{
	var avatarLable = document.createElement("div");
	avatarLable.setAttribute("class","avatar");

	var imgUrl = "https://gravatar.com/avatar/";
	avatarLable.innerHTML = "<img src=\"" + imgUrl + email_md5 + "\" alt>";
	main_label.appendChild(avatarLable);
}

function commentAppend(main_label,record)
{
	var commentBoxLable = document.createElement("div");
	commentBoxLable.setAttribute("class","comment-box");

	/* comment box head area star-area */
	var boxHead = document.createElement("div");
	boxHead.setAttribute("class","cbox-head clearfix");

	var authorName = document.createElement("span");
	authorName.setAttribute("class","cbauthor-name");
	authorName.innerText = record.nickname;

	var timeSpend = document.createElement("span");
	var timeSpace = Date.now() - record.create_at;
	var days = timeSpace / 1000 / 60 / 60 / 24;
	var f_days = Math.floor(days);
	var hours = timeSpace / 1000 / 60 / 60 - (24 * f_days);
	var f_hours = Math.floor(hours);
	var minutes = timeSpace / 1000 / 60 - (24 * 60 * f_days) - (60 * f_hours);
	var f_minutes = Math.floor(minutes);

	if(0 != f_days){ timeSpend.innerText = "" + f_days + " days ago"; }
	else if(0 != f_hours){ timeSpend.innerText = "" + f_hours + " hours ago"; } 
	else{ timeSpend.innerText = "" + f_minutes + " minutes ago"; }

	boxHead.appendChild(authorName);
	boxHead.appendChild(timeSpend);
	commentBoxLable.appendChild(boxHead);
	/* comment box head area end-area */

	/* coment box content area start-area */
	var boxContent = document.createElement("span");
	boxContent.setAttribute("class","cbox-conten");

	boxContent.innerHTML = record.content;
	commentBoxLable.appendChild(boxContent);

	main_label.appendChild(commentBoxLable);
}

function contentAppend(labels,record)
{
	var commentContainer = labels.commentContainer;
	var commentBlock = labels.commentBlock;
	var userBlock = labels.userBlock;
	var commentList = labels.commentList;
	var mainUserLable = document.createElement("div");

	mainUserLable.setAttribute("class","main-level clearfix");

	avatarAppend(mainUserLable,record.email_md5);
	commentAppend(mainUserLable,record);
	userBlock.appendChild(mainUserLable);

	/* state stretch */
	var O_CBHeight = commentBlock.clientHeight;
	var newHeight = O_CBHeight + 80;

	commentBlock.style.height = newHeight + "px";
	commentContainer.style.height = newHeight - 325 +  "px";
}

async function buttonClick(host,labels)
{
	var commentContainer = labels.commentContainer;
	var commentBlock = labels.commentBlock;
	var inputArea = labels.inputArea;
	var commentList = labels.commentList;

	var inputs = inputArea.getElementsByTagName("input");
	var nickname_label = inputs[0];
	var email_label = inputs[1];
	var email_md5 = CryptoJS.SHA256(email_label.value);
	var texts = inputArea.getElementsByTagName("textarea")[0];
	var nicknameValue = nickname_label.value;

	if(0 != nicknameValue.length)
	{
		/* current time */
		var textsContent = textareaBeautify(texts,texts.cols);
		var curTime = Date.now();
		var data = nicknameValue + ":" + email_label.value + ":" + email_md5 + ":" + curTime  + ":" + textsContent;

		if(true == await _PostRequest(host,"/api",data))
		{
			commentContainer.removeChild(commentList);
			commentBlock.style.height = "360px";
			commentContainer.style.height = "0px";

			var commentList = document.createElement("ul");

			commentList.setAttribute("class","comment-list");
			commentContainer.appendChild(commentList);

			labels.commentList = commentList;
			commentPreShow(host,labels);
		}
	}

	nickname_label.value = "";
	email_label.value = "";
	texts.value = "";
}

async function commentPreShow(host,labels)
{
	var responseData = await _GetRequest(host,"/database");

	if(false == responseData[0]){ return; }
	var data = responseData[1];
	var commentContainer = labels.commentContainer;
	var commentBlock = labels.commentBlock;
	var commentList = labels.commentList;
	var newLabels = new Object();
	var record = new Object();
	var array = data.split(",");

	array.pop();
	for(var i = 0;i < array.length;++i)
	{
		var sub = array[i];
		var nickname = "";
		var email = "";
		var email_md5 = "";
		var textsContent = "";
		var create_at = "";
		var member_ptr = 0;

		for(var j = 0;j < sub.length;++j)
		{
			const c = sub[j];

			if(":" == c && 4 != member_ptr){ ++member_ptr; }
			else
			{
				switch(member_ptr)
				{
					case 0: { nickname += c; break; }
					case 1: { email += c; break; }
					case 2: { email_md5 += c; break; }
					case 3: { create_at += c; break; }
					case 4: { textsContent += c; break; }
				}
			}
		}
		/* console.log(nickname + ":" + email + ":" + textsContent + ":" + create_at + "\n"); */
		var userBlock = document.createElement("li");
		var commentListFirstChild = commentList.firstChild;

		newLabels.commentContainer = commentContainer;
		newLabels.commentBlock = commentBlock;
		newLabels.userBlock = userBlock;
		newLabels.commentList = commentList;

		record.nickname = nickname;
		record.email = email;
		record.email_md5 = email_md5;
		record.create_at = create_at;
		record.content = textsContent;		

		contentAppend(newLabels,record);
		if(null == commentListFirstChild)
		{ 
			commentList.appendChild(userBlock);
			commentList.style.display = "block";
		}
		else{ commentList.insertBefore(userBlock,commentListFirstChild); }
	}
}

window.onload = function(){
	const _host = "192.168.80.143";
	/* class="comment-block" */
	var commentBlock = document.getElementById("comment-block");
	var inputBlock = commentBlock.firstElementChild;/* class="input-block" */

	/*** input-block ***/
	/******************************/
	/* class="input-area" */
	var inputArea = inputBlock.firstElementChild;
	/* class="text-block" */
	var buttonArea = inputArea.lastElementChild;
	/* class="commit" */
	var commitButton = buttonArea.getElementsByTagName("button")[0];
	/******************************/

	var newlabels = new Object();
	/* class="comment-container" */
	var commentContainer = inputBlock.nextElementSibling;
	var commentList = document.createElement("ul");

	commentList.setAttribute("class","comment-list");
	commentContainer.appendChild(commentList);
	
	newlabels.commentContainer = commentContainer;
	newlabels.commentBlock = commentBlock;
	newlabels.commentList = commentList;
	newlabels.inputArea = inputArea;

	commentPreShow(_host,newlabels);
	commitButton.addEventListener(
		"click",
		function(){ buttonClick(_host,newlabels); },
		false
	);
};
