async function _PostRequest(host,url,data)
{
	const _URL = "http://" + host + url;
	var response = await fetch(_URL,{
		method: "POST",
		headers:{
			"Content-Type": "text/plain",
		},
		body: data,
	});
	if(!response.ok){ return false; }
	return true;
}

async function _GetRequest(host,url)
{
	const _URL = "http://" + host + url;
	var response = await fetch(_URL);
    
	if(!response.ok){ return [false,""]; }
    
	return [true,await response.text()];
}

function textareaBeautify(textsLable,cols){
	var cache = "";
	var texts = (textsLable.value).replace(/\r|\n/g,"");
	var textLength = texts.length;

	for(var i = 0;i < textLength;++i){
		if(0 != cache.length && 0 === cache.length % cols)
			cache += "<br/>";
		cache += texts[i];
	};
	return cache;
}

function contentAppend(labels,nickNameValue,textsContent,creatat){
	var commentContainer = labels[0];
	var commentBlock = labels[1];
	var userBlock = labels[2];
	var commentList = labels[3];
	var mainUserLable = document.createElement("div");
	mainUserLable.setAttribute("class","main-level clearfix");

	/* avatar area  start-area */
	var avatarLable = document.createElement("div");
	avatarLable.setAttribute("class","avatar");

	var imgUrl = "./img/img.png";
	avatarLable.innerHTML = "<img src=\"" + imgUrl + "\"alt>";
	mainUserLable.appendChild(avatarLable);
	/* avatar area  end-area */

	var commentBoxLable = document.createElement("div");
	commentBoxLable.setAttribute("class","comment-box");

	/* comment box of coment area start-area */
	/* comment box head area star-area */
	var boxHead = document.createElement("div");
	boxHead.setAttribute("class","cbox-head clearfix");

	var authorName = document.createElement("span");
	authorName.setAttribute("class","cbauthor-name");
	authorName.innerText = nickNameValue;

	var timeSpend = document.createElement("span");
	var timeSpace = Date.now() - creatat;
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

	boxContent.innerHTML = textsContent;
	commentBoxLable.appendChild(boxContent);
	/* comment box of coment area end-area */

	mainUserLable.appendChild(commentBoxLable);
	userBlock.appendChild(mainUserLable);

	/* state stretch */
	var O_CBHeight = commentBlock.clientHeight;
	var newHeight = O_CBHeight + 80;

	commentBlock.style.height = newHeight + "px";
	commentContainer.style.height = newHeight - 325 +  "px";
}

async function buttonClick(host,labels)
{
	var commentContainer = labels[0];
	var commentBlock = labels[1];
	var inputArea = labels[2];
	var commentList = labels[3];

	var inputs = inputArea.getElementsByTagName("input");
	var nickName = inputs[0];
	var email = inputs[1];
	var texts = inputArea.getElementsByTagName("textarea")[0];

	var nickNameValue = nickName.value;
	if(0 != (nickNameValue.length))
	{
		/* current time */
		var textsContent = textareaBeautify(texts,texts.cols);
		var curTime = Date.now();
		var data = nickNameValue + ":" + email.value + ":" + textsContent + ":" + curTime; 

		if(true ==  await _PostRequest(host,"/api",data))
		{
			var userBlock = document.createElement("li");
			var commentListFirstChild = commentList.firstChild;
			var newLabels = [commentContainer,commentBlock,userBlock,commentList];

			contentAppend(newLabels,nickNameValue,textsContent,curTime);
			if(null == commentListFirstChild)
			{ 
				commentList.appendChild(userBlock); 
				commentList.style.display = "block";
			}
			else{ commentList.insertBefore(userBlock,commentListFirstChild); }
		}
	}

	nickName.value = "";
	email.value = "";
	texts.value = "";
}

function commentPreShow(data,labels)
{
	var commentContainer = labels[0];
	var commentBlock = labels[1];
	var commentList = labels[2];
	var array = data.split(",");
	array.pop();

	for(var i = 0;i < array.length;++i)
	{
		var sub = array[i];
		var nickName = "";
		var email = "";
		var textsContent = "";
		var createat = "";
		var j = 0;
		while(j < sub.length && ':' != sub[j])
			nickName += sub[j++];
		for(++j;j < sub.length && ':' != sub[j];++j)
			email += sub[j];
		for(++j;j < sub.length && ':' != sub[j];++j)
			textsContent += sub[j];
		for(++j;j < sub.length;++j){ createat += sub[j]; }

/*	console.log(nickName + ":" + email + ":" + textsContent + ":" + createat + "\n"); */
		var userBlock = document.createElement("li");
		var commentListFirstChild = commentList.firstChild;
		var newLabels = [commentContainer,commentBlock,userBlock,commentList];

		contentAppend(newLabels,nickName,textsContent,createat);
		if(null == commentListFirstChild)
		{ 
			commentList.appendChild(userBlock);
			commentList.style.display = "block";
		}
		else{ commentList.insertBefore(userBlock,commentListFirstChild); }
	}
}

window.onload = async function(){
	const _host = "192.168.80.140";
	var commentBlock = document.getElementById("comment-block");/* class="comment-block" */

	var inputBlock = commentBlock.firstElementChild;/* class="input-block" */
	/* input-block */
	var inputArea = inputBlock.firstElementChild;/* class="input-area" */
	var buttonArea = inputArea.lastElementChild;/* class="text-block" */
	var commitButton = buttonArea.getElementsByTagName("button")[0];/* class="commit" */
	/***************/
	var commentContainer = inputBlock.nextElementSibling;/* class="comment-container" */

	var responseData = await _GetRequest(_host,"/database");
	if(false == responseData[0]){ return; }

	var commentList = document.createElement("ul");
	commentList.setAttribute("class","comment-list");
	commentContainer.appendChild(commentList);
	commentPreShow(responseData[1],[commentContainer,commentBlock,commentList]);

	commitButton.addEventListener("click",function(){
		buttonClick(_host,[commentContainer,commentBlock,inputArea,commentList]);
	},false);
};
