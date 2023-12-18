# Fetch and (several) other commands for gccg.
#
# Copyright (C) 2015 Quadragon <kalalal@hotmail.it>
# 
# Adapted from scripts by Russell Jones <questiondesk@gmail.com> and Neil Moore <neil@s-z.org>.
#

if(msg.box != NULL)
  Msg("Loading {gold}Zonechange.command");

def PlayCommandFetch
{
  if(length(ARG)<1)
    Msg("{red}Must specify the name of a card, see /help fetch.");
  else if(deck(deck)==(,))
    Msg("{red}Deck is empty.");
  else
  {
    push(foundInDeck);
    push(exactFound);
    push(c);
    push(name);
    push(field);

    foundInDeck = (,);
    name=join(ARG," ");
    
    field=(,);
    
    for(c)(keys(decks{deck.name}))
    {
      field=field+decks{deck.name}{c};
    }
    
    for(c)(field)
    {
      if(find(lc(name), lc(canonical_name(c)))!=NULL)
      {
        if(find(c,foundInDeck)==NULL) 
          foundInDeck = foundInDeck + (c,);
      }
      if(lc(name) == lc(name(c)))
      {
        exactFound = c;
        break;
      }
    }

    if(exactFound)
      foundInDeck = (exactFound,);
    if(foundInDeck == (,))
      Msg("{red}Cannot find {brown}" + name + " {red}in your deck.");
    else
    {
      for(c)(foundInDeck)
      {
        Send("Transfer", (1, ("deck.search", deck, c), ("active", me)));
      }
    }

    Send("ShuffleDeck", deck);

    name=pop();
    c=pop();
    exactFound=pop();
    foundInDeck=pop();
    field=pop();
  }  
}


def PlayCommandFetchp
{
  if(length(ARG)<1)
    Msg("{red}Must specify the name of a card, see /help fetchp.");
  else if(deck(deck)==(,))
    Msg("{red}Deck is empty.");
  else
  {
    push(foundInDeck);
    push(exactFound);
    push(c);
    push(name);
    push(field);

    foundInDeck = (,);
    name=join(ARG," ");
    
    field=(,);
    
    for(c)(keys(decks{deck.name}))
    {
      field=field+decks{deck.name}{c};
    }
    
    for(c)(field)
    {
      if(find(lc(name), lc(canonical_name(c)))!=NULL)
      {
        if(find(c,foundInDeck)==NULL) 
          foundInDeck = foundInDeck + (c,);
      }
      if(lc(name) == lc(name(c)))
      {
        exactFound = c;
        break;
      }
    }

    if(exactFound)
      foundInDeck = (exactFound,);
    if(foundInDeck == (,))
      Msg("{red}Cannot find {brown}" + name + " {red}in your deck.");
    else
    {
      for(c)(foundInDeck)
      {
        Send("Transfer", (1, ("deck.search", deck, c), ("play", 0)));
      }
    }

    Send("ShuffleDeck", deck);

    name=pop();
    c=pop();
    exactFound=pop();
    foundInDeck=pop();
    field=pop();
  }  
}

def PlayCommandFetchx
{
  if(length(ARG)<1)
    Msg("{red}Must specify the name of a card, see /help fetchx.");
  else if(deck(deck)==(,))
    Msg("{red}Deck is empty.");
  else
  {
    push(foundInDeck);
    push(exactFound);
    push(c);
    push(name);
    push(field);

    foundInDeck = (,);
    name=join(ARG," ");
    
    field=(,);
    
    for(c)(keys(decks{deck.name}))
    {
      field=field+decks{deck.name}{c};
    }
    
    for(c)(field)
    {
      if(find(lc(name), lc(canonical_name(c)))!=NULL)
      {
        if(find(c,foundInDeck)==NULL) 
          foundInDeck = foundInDeck + (c,);
      }
      if(lc(name) == lc(name(c)))
      {
        exactFound = c;
        break;
      }
    }

    if(exactFound)
      foundInDeck = (exactFound,);
    if(foundInDeck == (,))
      Msg("{red}Cannot find {brown}" + name + " {red}in your deck.");
    else
    {
      for(c)(foundInDeck)
      {
        Send("Transfer", (1, ("deck.search", deck, c), ("deck.top",deck.objects[me]{'exile'})));
      }
    }

    Send("ShuffleDeck", deck);

    name=pop();
    c=pop();
    exactFound=pop();
    foundInDeck=pop();
    field=pop();
  }  
}

def PlayCommandRevive
{
  if(length(ARG)<1)
    Msg("{red}Must specify the name of a card, see /help revive.");
  else if(deck(discard)==(,))
    Msg("{red}Graveyard is empty.");
  else
  {
    push(foundInDeck);
    push(exactFound);
    push(c);
    push(name);

    foundInDeck = (,);
    name=join(ARG," ");
    
    for(c)(deck(discard))
    {
      if(find(lc(name), lc(canonical_name(c)))!=NULL)
      {
        if(find(c,foundInDeck)==NULL) 
          foundInDeck = foundInDeck + (c,);
      }
      if(lc(name) == lc(name(c)))
      {
        exactFound = c;
        break;
      }
    }

    if(exactFound)
      foundInDeck = (exactFound,);
    if(foundInDeck == (,))
      Msg("{red}Cannot find {brown}" + name + " {red}in your graveyard.");
    else
    {
      for(c)(foundInDeck)
      {
        Send("Transfer", (1, ("deck.search", discard, c), ("play", 0)));
      }
    }

    name=pop();
    c=pop();
    exactFound=pop();
    foundInDeck=pop();
  }  
}

def PlayCommandMassrevive
{
  if(length(ARG)<1)
    Msg("{red}Must specify one or more types, subtypes or colors, see /help massrevive.");
  else if(deck(discard)==(,))
    Msg("{red}Graveyard is empty.");
  else
  {
    push(v);
    push(c);
    
    v=(,);
        
    for(c)(deck(discard)){
      if(find(lc(join(ARG," ")), lc(join((Attr("color",c), Attr("subtype",c), Attr("type",c))," "))) != NULL){
       v=v+(c,);
      }
    }
    
    for(c)(v){
      Send("Transfer",(1,("deck.search", discard, c),("play", 0)));
    }

    v=pop();
    c=pop();
  }  
}

def PlayCommandRegrow
{
  if(length(ARG)<1)
    Msg("{red}Must specify the name of a card, see /help regrow.");
  else if(deck(discard)==(,))
    Msg("{red}Graveyard is empty.");
  else
  {
    push(foundInDeck);
    push(exactFound);
    push(c);
    push(name);

    foundInDeck = (,);
    name=join(ARG," ");
    
    for(c)(deck(discard))
    {
      if(find(lc(name), lc(canonical_name(c)))!=NULL)
      {
        if(find(c,foundInDeck)==NULL) 
          foundInDeck = foundInDeck + (c,);
      }
      if(lc(name) == lc(name(c)))
      {
        exactFound = c;
        break;
      }
    }

    if(exactFound)
      foundInDeck = (exactFound,);
    if(foundInDeck == (,))
      Msg("{red}Cannot find {brown}" + name + " {red}in your graveyard.");
    else
    {
      for(c)(foundInDeck)
      {
        Send("Transfer", (1, ("deck.search", discard, c), ("hand", me)));
      }
    }

    name=pop();
    c=pop();
    exactFound=pop();
    foundInDeck=pop();
  }  
}

def PlayCommandMassregrow
{
  if(length(ARG)<1){
    Msg("{red}Must specify one or more types, subtypes or colors, or ALL see /help massregrow.");
  } else if(deck(discard)==(,)){
    Msg("{red}Graveyard is empty.");
  } else {
    push(v);
    push(c);
    
    if (uc(ARG[0])=="ALL"){
      v=(deck(discard));
    } else {
      v=(,);
      
      for(c)(deck(discard)){
        if(find(lc(join(ARG," ")), lc(join((Attr("color",c), Attr("subtype",c), Attr("type",c))," "))) != NULL){
        v=v+(c,);
        }
      }
    }
    
    for(c)(v){
      Send("Transfer",(1,("deck.search", discard, c),("hand",me)));
    }

    v=pop();
    c=pop();
  }  
}

def PlayCommandDredge
{
  if(length(ARG)<1)
    Msg("{red}Must specify the name of a card, see /help dredge.");
  else if(deck(discard)==(,))
    Msg("{red}Graveyard is empty.");
  else
  {
    push(foundInDeck);
    push(exactFound);
    push(c);
    push(name);

    foundInDeck = (,);
    name=join(ARG," ");
    
    for(c)(deck(discard))
    {
      if(find(lc(name), lc(canonical_name(c)))!=NULL)
      {
        if(find(c,foundInDeck)==NULL && find("Dredge",text(c))!=NULL) 
          foundInDeck = foundInDeck + (c,);
      }
      if(lc(name) == lc(name(c)))
      {
        exactFound = c;
        break;
      }
    }

    if(exactFound)
      foundInDeck = (exactFound,);
    if(foundInDeck == (,))
      Msg("{red}Cannot find a card with both {brown}" + name + " {red}in its name and {brown}Dredge {red}in its text, in your graveyard.");
    else if(length(foundInDeck) > 1)
      Msg("{red}" + length(foundInDeck) + " {red}cards found, please, refine your filter.");
    else
    {
      Say("{cyan}Dredging {gold}"+name(foundInDeck[0]));
      Send("Transfer",(substr(text(foundInDeck[0]),find("Dredge",text(foundInDeck[0]))+7,1),("deck.top",deck),("deck.top",discard)));
      Send("Transfer", (1, ("deck.search", discard, foundInDeck[0]), ("hand", me)));
    }

    name=pop();
    c=pop();
    exactFound=pop();
    foundInDeck=pop();
  }  
}

def PlayCommandEntomb
{
  if(length(ARG)<1)
    Msg("{red}Must specify the name of a card, see /help entomb.");
  else if(deck(deck)==(,))
    Msg("{red}Deck is empty.");
  else
  {
    push(foundInDeck);
    push(exactFound);
    push(c);
    push(name);
    push(field);

    foundInDeck = (,);
    name=join(ARG," ");
    
    field=decks{deck.name}{"deck"};
    
    if (has_entry("sideboard",decks{deck.name})){
      field=field+decks{deck.name}{"sideboard"};
    }    
    if (has_entry("exile",decks{deck.name})){
      field=field+decks{deck.name}{"exile"};
    }
    
    for(c)(field)
    {
      if(find(lc(name), lc(canonical_name(c)))!=NULL)
      {
        if(find(c,foundInDeck)==NULL) 
          foundInDeck = foundInDeck + (c,);
      }
      if(lc(name) == lc(name(c)))
      {
        exactFound = c;
        break;
      }
    }

    if(exactFound)
      foundInDeck = (exactFound,);
    if(foundInDeck == (,))
      Msg("{red}Cannot find {brown}" + name + " {red}in your deck.");
    else
    {
      for(c)(foundInDeck)
      {
        Send("Transfer", (1, ("deck.search", deck, c), ("deck.top",discard)));
      }
    }

    Send("ShuffleDeck", deck);

    name=pop();
    c=pop();
    exactFound=pop();
    foundInDeck=pop();
    field=pop();
  }  
}

def PlayCommandGtop
{
  if(length(ARG)<1)
    Msg("{red}Must specify the name of a card, see /help gtop.");
  else if(deck(discard)==(,))
    Msg("{red}Graveyard is empty.");
  else
  {
    push(foundInDeck);
    push(exactFound);
    push(c);
    push(name);

    foundInDeck = (,);
    name=join(ARG," ");
    
    for(c)(deck(discard))
    {
      if(find(lc(name), lc(canonical_name(c)))!=NULL)
      {
        if(find(c,foundInDeck)==NULL) 
          foundInDeck = foundInDeck + (c,);
      }
      if(lc(name) == lc(name(c)))
      {
        exactFound = c;
        break;
      }
    }

    if(exactFound)
      foundInDeck = (exactFound,);
    if(foundInDeck == (,))
      Msg("{red}Cannot find {brown}" + name + " {red}in your graveyard.");
    else
    {
      for(c)(foundInDeck)
      {
        Send("Transfer", (1, ("deck.search", discard, c), ("deck.top",discard)));
      }
    }

    name=pop();
    c=pop();
    exactFound=pop();
    foundInDeck=pop();
  }  
}

def PlayCommandXtop
{
  push(e);
  e = MyDeck("exile");
  if(length(ARG)<1)
    Msg("{red}Must specify the name of a card, see /help xtop.");
  else if(!e)
    Msg("{red}Exile is empty.");
  else
  {
    push(foundInDeck);
    push(exactFound);
    push(c);
    push(name);

    foundInDeck = (,);
    name=join(ARG," ");
    
    for(c)(deck(e))
    {
      if(find(lc(name), lc(canonical_name(c)))!=NULL)
      {
        if(find(c,foundInDeck)==NULL) 
          foundInDeck = foundInDeck + (c,);
      }
      if(lc(name) == lc(name(c)))
      {
        exactFound = c;
        break;
      }
    }

    if(exactFound)
      foundInDeck = (exactFound,);
    if(foundInDeck == (,))
      Msg("{red}Cannot find {brown}" + name + " {red}in your exile.");
    else
    {
      for(c)(foundInDeck)
      {
        Send("Transfer", (1, ("deck.search", e, c), ("deck.top",e)));
      }
    }

    name=pop();
    c=pop();
    exactFound=pop();
    foundInDeck=pop();
  }
  e=pop();
}

def PlayCommandFlash
{
  if(length(ARG)<1)
    Msg("{red}Must specify the name of a card, see /help flash.");
  else if(deck(discard)==(,))
    Msg("{red}Graveyard is empty.");
  else
  {
    push(foundInDeck);
    push(exactFound);
    push(c);
    push(name);

    foundInDeck = (,);
    name=join(ARG," ");
    
    for(c)(deck(discard))
    {
      if(find(lc(name), lc(canonical_name(c)))!=NULL)
      {
        if(find(c,foundInDeck)==NULL) 
          foundInDeck = foundInDeck + (c,);
      }
      if(lc(name) == lc(name(c)))
      {
        exactFound = c;
        break;
      }
    }

    if(exactFound)
      foundInDeck = (exactFound,);
    if(foundInDeck == (,))
      Msg("{red}Cannot find {brown}" + name + " {red}in your graveyard.");
    else
    {
      for(c)(foundInDeck)
      {
        Send("Transfer", (1, ("deck.search", discard, c), ("deck.top", MyDeck("exile"))));
      }
    }
    
    name=pop();
    c=pop();
    exactFound=pop();
    foundInDeck=pop();
  }  
}

def PlayCommandKillall
{
  push(v);
  push(c);
  
  if(length(ARG)==0)
    v=inplay(me);
  else
    v=select("c=card(#); find(lc(join(ARG,\" \")), lc(join((Attr(\"color\",c), Attr(\"subtype\",c), Attr(\"type\",c)),\" \"))) != NULL", inplay(me));
  
  for(c)(v){
    Send("Transfer",(1,("table",c),("deck.top",discard)));
   }
   
   v=pop();
   c=pop();
}

def PlayCommandLiving
{
  call("PlayCommandKillall","creature");
  call("PlayCommandMassrevive",("creature",));
}

def PlayCommandTop
{
  Send("Transfer",(ARG[0],("deck.top",deck),("active",me)));
}

def PlayCommandDraw
{
  if(length(ARG)>0)
    E=toint(ARG[0]);
  else
    E=0;
  if (E>0)
    Send("Transfer",(ARG[0],("deck.top",deck),("hand",me)));
  else
    Send("DeclareResult",("draw",join(ARG," ")));
}

def PlayCommandMill
{
  Send("Transfer",(ARG[0],("deck.top",deck),("deck.top",discard)));
}

def PlayCommandXlib
{
  Send("Transfer",(ARG[0],("deck.top",deck),("deck.top",deck.objects[me]{"exile"})));
}

def PlayCommandNuke
{
  Send("Transfer",(ARG[0],("deck.top",deck),("out.of.play",)));
}

def PlayCommandLibfill
{
  num = -1;
  for(i)(length(player.name))
  {
    if(player.name[i][1]==ARG[1])
      num = i;
  }
  if(num == -1)
    Msg("{red}"+ARG[1]+" is not a player in this game.")
  else
    Send("Transfer",(ARG[0],("deck.top",deck),("deck.top",deck.objects[num]{"opponent deck"})));
}

def PlayCommandXg
{
  Send("Transfer",(length(deck(discard)),("deck.top",discard),("deck.top",deck.objects[me]{"exile"})));
}

def PlayCommandPickup
{
  Send("Transfer",(length(cardbox(active[me])),("active",me,0),("hand",me)));
}

def PlayCommandScrap
{
  Send("Transfer",(length(cardbox(active[me])),("active",me,0),("deck.top",deck.objects[me]{"exile"})));
}

def PlayCommandDiscard
{
  Send("Transfer",(length(cardbox(active[me])),("active",me,0),("deck.top",deck.objects[me]{"discard pile"})));
}

def CommandRoll
{
  if(ARG==(,))
    Send("Roll","2d6");
  else
    Send("Roll",ARG[0]);
}

def PlayCommandSide
{
  Send("Transfer",(length(cardbox(active[me])),("active",me,0),("deck.top",deck.objects[me]{"sideboard"})));
}

def PlayCommandPlayall
{
  push(v);
  push(c);
  
  card.group=(,);
  
  if(length(ARG)==0){
    v=inplay(me);
  } else {
    v=select("c=card(#); find(lc(join(ARG,\" \")), lc(join((Attr(\"color\",c), Attr(\"subtype\",c), Attr(\"type\",c)),\" \"))) != NULL", inplay(me));
  }
  for(c)(v){
    Send("Transfer",(1,("table",c),("play", 0)));
  }
  v=pop();
  c=pop();
}

def PlayCommandFixall
{
  push(c);
  push(counter);
  push(field);
  push(namelist);
  push(v);
  push(xy);
  push(temp);
  push(azinplay);
  push(ltrinplay);
  push(stack);
  push(unkname);
  push(unkloc);
  push(findspace.facedown);
	
  counter=(0,0,0,0);
	namelist=(,);
  findspace.facedown=0;
  unkloc=(,);
	
#	
# Assign unique cards in inplay(me) to ltrinplay.
#

	ltrinplay=(("land",(,)),("permanent",(,)),("spell",(,)),("wenchantment",(,)));
	
	for(c)(inplay(me))
  {
    if(name(card(c))=="_unknown_")
    {
      findspace.facedown=1;
      unkloc=unkloc+(counter[1],);
    } else {
      findspace.facedown=0;
    }
		if(find((Category(card(c)))[1],namelist)==NULL)
    {
			namelist=namelist+(((Category(card(c)))[1]),);
			ltrinplay{(Category(card(c)))[0]}=ltrinplay{(Category(card(c)))[0]}+(c,);
      if((Category(card(c)))[0]=="land")
      {
				counter[0]=counter[0]+1;
			}
			if((Category(card(c)))[0]=="permanent")
      {
				counter[1]=counter[1]+1;
			}
      if((Category(card(c)))[0]=="spell")
      {
				counter[2]=counter[2]+1;
			}
      if((Category(card(c)))[0]=="wenchantment")
      {
        counter[3]=counter[3]+1;
      }
		}
	}
  
	
#	
# Order ltrinplay and change object numbers to card names.
# The first step is likely unnecessary, due to how cards are added to ltrinplay.
#

  for(c)(keys(ltrinplay))
  {
		ltrinplay{c}=sort_fn("(center_of(#))[0]",ltrinplay{c});
	}


	for(c)(keys(ltrinplay))
  {
		for(v)(length(ltrinplay{c}))
    {
			ltrinplay{c}[v]=name(card(ltrinplay{c}[v]));
		}
	}
	
#
# Assign inplay(me) to azinplay and order the latter alphabetically.
#

  azinplay=inplay(me);
  
  azinplay=sort_fn('strreplace(name(card(#)),"_unknown_","0")',azinplay);
  
#	
# Define form of card.group.
#
	
	card.group=(,);
  field=0;
	
	if(counter[0]>0)
  {
		card.group=card.group+(("land",(("free",(0,(0,),((0,0),))),)),);
		if(counter[0]>1)
    {
			for(v)(counter[0]-1)
      {
				card.group[field][1]=card.group[field][1]+(("free",(0,(0,),((0,0),))),);
			}
		}
    field=field+1;
  }
  if(counter[1]>0)
  {
		card.group=card.group+(("permanent",(("free",(0,(0,),((0,0),))),)),);
		if (counter[1]>1)
    {
			for(v)(counter[1]-1)
      {
				card.group[field][1]=card.group[field][1]+(("free",(0,(0,), ((0,0),))),);
			}
		}
    field=field+1;
	}
  if(counter[2]>0)
  {
		card.group=card.group+(("spell",(("",(0,(0,),((0,0),))),)),);
    field=field+1;
	}
  if(counter[3]>0)
  {
    card.group=card.group+(("wenchantment",(("free",(0,(0,),((0,0),))),)),);
		if (counter[3]>1)
    {
			for(v)(counter[3]-1)
      {
				card.group[field][1]=card.group[field][1]+(("free",(0,(0,),((0,0),))),);
			}
		}
  }
	
#	
# Fill out contents of card.group.
#

  counter=(0,0,0,0);
	namelist=(,);
  unkname=current_time();
  findspace.facedown=0;
	
	for(c)(azinplay)
  {
    stack=0;
    if(find((Category(card(c)))[1],namelist)!=NULL)
    {
      stack=1;
    }
    if(name(card(c))=="_unknown_")
    {
      findspace.facedown=1;
      stack=0;
      unkname=unkname+1;
    } else {
      findspace.facedown=0;
    }
		xy=center_of(c,1);
    if((Category(card(c)))[0]=="land")
    {
			if(stack==0)
      {
				namelist=namelist+((Category(card(c)))[1],);
				card.group{(Category(card(c)))[0]}[counter[0]]=((Category(card(c)))[1],(find((Category(card(c)))[1],ltrinplay{(Category(card(c)))[0]}),(c,),(xy,)));
				counter[0]=counter[0]+1;
			} else {
				card.group{(Category(card(c)))[0]}{(Category(card(c)))[1]}[1]=(card.group{(Category(card(c)))[0]}{(Category(card(c)))[1]})[1]+(c,);
        card.group{(Category(card(c)))[0]}{(Category(card(c)))[1]}[2]=(card.group{(Category(card(c)))[0]}{(Category(card(c)))[1]})[2]+(xy,);
			}
		}
		if((Category(card(c)))[0]=="permanent")
    {
			if(stack==0)
      {
        if(findspace.facedown==1)
        {
          namelist=namelist+(unkname,);
          card.group{(Category(card(c)))[0]}[counter[1]]=(unkname,(unkloc[toint(unkname-current_time()-1)],(c,),(xy,)));
        } else {
          namelist=namelist+((Category(card(c)))[1],);
          card.group{(Category(card(c)))[0]}[counter[1]]=((Category(card(c)))[1],(find((Category(card(c)))[1],ltrinplay{(Category(card(c)))[0]}),(c,),(xy,)));
        }
        counter[1]=counter[1]+1;
			} else {
				card.group{(Category(card(c)))[0]}{(Category(card(c)))[1]}[1]=(card.group{(Category(card(c)))[0]}{(Category(card(c)))[1]})[1]+(c,);
        card.group{(Category(card(c)))[0]}{(Category(card(c)))[1]}[2]=(card.group{(Category(card(c)))[0]}{(Category(card(c)))[1]})[2]+(xy,);
			}
		}
    if((Category(card(c)))[0]=="spell")
    {
			if(counter[2]==0)
      {
				card.group{(Category(card(c)))[0]}[0]=("",(0,(c,),(xy,)));
				counter[2]=counter[2]+1;
			} else {
				card.group{(Category(card(c)))[0]}{(Category(card(c)))[1]}[1]=(card.group{(Category(card(c)))[0]}{(Category(card(c)))[1]})[1]+(c,);
        card.group{(Category(card(c)))[0]}{(Category(card(c)))[1]}[2]=(card.group{(Category(card(c)))[0]}{(Category(card(c)))[1]})[2]+(xy,);
			}
		}
    if((Category(card(c)))[0]=="wenchantment")
    {
			if(stack==0)
      {
				namelist=namelist+((Category(card(c)))[1],);
				card.group{(Category(card(c)))[0]}[counter[3]]=((Category(card(c)))[1],(find((Category(card(c)))[1],ltrinplay{(Category(card(c)))[0]}),(c,),(xy,)));
				counter[3]=counter[3]+1;
			} else {
				card.group{(Category(card(c)))[0]}{(Category(card(c)))[1]}[1]=(card.group{(Category(card(c)))[0]}{(Category(card(c)))[1]})[1]+(c,);
        card.group{(Category(card(c)))[0]}{(Category(card(c)))[1]}[2]=(card.group{(Category(card(c)))[0]}{(Category(card(c)))[1]})[2]+(xy,);
			}
		}
	}
  
	

#	
# Clear variables.
#
	
  findspace.facedown=pop();
  unkloc=pop();
  unkname=pop();
  stack=pop();
  ltrinplay=pop();
	azinplay=pop();
	temp=pop();
  xy=pop();
	v=pop();
	namelist=pop();
  field=pop();
  counter=pop();
	c=pop();
}


HELP{"play"}{"fixall"}=("","Fix all cards",NULL,
"Fixes all your cards to the table.");

HELP{"play"}{"playall"}=("[type]","Play all cards",NULL,
"Play all your cards from the table. If {yellow}<type>{white} is given as a color, subtype, or type, only cards of that quality are played.");

HELP{"play"}{"fetch"}=("name","Fetch a card from library",NULL,
"If {yellow}<name>{white} is the full name of a card from your library, set one of those cards aside. Otherwise, set aside one of each card whose name contains {yellow}<name>{white}.  Shuffle your library afterwards.");

HELP{"play"}{"fetchp"}=("name","Play a card from library",NULL,
"If {yellow}<name>{white} is the full name of a card from your library, put one of those cards to play. Otherwise, put one of each card whose name contains {yellow}<name>{white} to play.  Shuffle your library afterwards.");

HELP{"play"}{"fetchx"}=("name","Exile a card from library",NULL,
"If {yellow}<name>{white} is the full name of a card from your library, put one of those cards to exile. Otherwise, exile one of each card whose name contains {yellow}<name>{white}.  Shuffle your library afterwards.");

HELP{"play"}{"entomb"}=("name","Put a card from library to graveyard",NULL,
"If {yellow}<name>{white} is the full name of a card from your library, put one of those cards to the graveyard. Otherwise, put one of each card whose name contains {yellow}<name>{white} to the graveyard.  Shuffle your library afterwards.");

HELP{"play"}{"revive"}=("name","Play a card from graveyard",NULL,
"If {yellow}<name>{white} is the full name of a card from your graveyard, put one of those cards to play. Otherwise, put one of each card whose name contains {yellow}<name>{white} from graveyard to play.");

HELP{"play"}{"massrevive"}=("type","Play cards from graveyard",NULL,
"Put each card whose color, subtype, or type contains {yellow}<type>{white} from graveyard to play.");

HELP{"play"}{"regrow"}=("name","Get a card from graveyard",NULL,
"If {yellow}<name>{white} is the full name of a card from your graveyard, put one of those cards to hand. Otherwise, put one of each card whose name contains {yellow}<name>{white} from graveyard to hand.");

HELP{"play"}{"massregrow"}=("[ {orange}all{yellow} ] | type","Get cards from graveyard",NULL,
"Put each card whose color, subtype, or type contains {yellow}<type>{white} from graveyard to hand. If {orange}all{white} is give as an argument, all cards in graveyard are put to hand.");

HELP{"play"}{"dredge"}=("name","Dredge a card from graveyard",NULL,
"If {yellow}<name>{white} is the full name of a card or part of the name of a single card with dredge in your graveyard, put one of those cards to hand and mill for the correct amount.");

HELP{"play"}{"flash"}=("name","Exile a card from graveyard",NULL,
"If {yellow}<name>{white} is the full name of a card from your graveyard, put one of those cards to exile. Otherwise, put one of each card whose name contains {yellow}<name>{white} from graveyard to exile.");

HELP{"play"}{"xg"}=("","Exile all cards from graveyard",NULL,
"Move all cards from graveyard into the exile pile.");

HELP{"play"}{"pickup"}=("","Move set-aside cards to hand",NULL,
"Pick up all the cards you have set aside and put them into your hand.");

HELP{"play"}{"scrap"}=("","Exile set-aside cards",NULL,
"Take all the cards you have set aside and toss them into exile.");

HELP{"play"}{"discard"}=("","Discard set-aside cards",NULL,
"Take all the cards you have set aside and toss them into your graveyard.");

HELP{"play"}{"side"}=("","Side out set-aside cards",NULL,
"Take all the cards you have set aside and put them in your the sideboard.");

HELP{"play"}{"gtop"}=("name","Put a card to top of graveyard",NULL,
"Put a card in the graveyard whose name contains {yellow}<name>{white} to the top of that pile.");

HELP{"play"}{"xtop"}=("name","Put a card to top of exile",NULL,
"Put a card in exile whose name contains {yellow}<name>{white} to the top of that pile.");

HELP{"play"}{"killall"}=("[type]","Move all cards to graveyard",NULL,
"Move all of your cards from the table to the graveyard. If {yellow}<type>{white} is given as a color, subtype, or type, only cards of that quality are moved to the graveyard.");

HELP{"play"}{"living"}=("","Living End/Death",NULL,
"Sacrifices all the creatures in play and simultaneously revives all the creatures in the graveyard.");

HELP{"play"}{"top"}=("num","Set aside cards from library",NULL,
"Put {yellow}<num>{white} cards from top of library to aside.");

HELP{"play"}{"draw"}=("options | num","Put cards from library to hand or declare draw",NULL,
"If no argument (or an argument which is not an integer) is offered, declare the result of the game to be a draw. If someone has already declared the result, new declaration overrides it. If the argument is an integer, put {yellow}<num>{white} cards from top of library to hand.");

HELP{"play"}{"mill"}=("num","Put cards from library to graveyard",NULL,
"Put {yellow}<num>{white} cards from top of library to graveyard.");

HELP{"play"}{"xlib"}=("num","Put cards from library to exile",NULL,
"Put {yellow}<num>{white} cards from top of library to exile.");

HELP{"play"}{"nuke"}=("num","Remove cards from library",NULL,
"Put {yellow}<num>{white} cards from top of library to out of play.");

HELP{"play"}{"libfill"}=("num> <name","Give cards to another library",NULL,
"Put {yellow}<num>{white} cards from the top of your library to the top of {yellow}<name>{white}'s library.");
