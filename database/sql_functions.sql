/* 
All functions are created for use in "NETB375 Programing Practise" project for team 1.
GitHub link: https://github.com/borislav-draganov/NETB375

Author: Martin Dimitrov - Faculty number: F61821
*/

/* 								START OF SEARCH FUNCTIONS SECTION								 */

/* 
****************************************************************************************************************************************************************
********								Search By Keyword v.1.0								********
******** This function should be called when a search is performed by keyword.										********
******** Function usage:																********
********	select * from searchByKeyword('%keyword%');												********
******** where keyword is an actual keyword passed by the user.												********
********																		********
******** It is recomended to use percent signs around the word when creating the query as it's possible that the user has entered only partial text.	********
******** Example code:																	********
********	qry = "SELECT * from searchByKeyword('%" + myString + "%');" ;										********
********																		********
******** Don't forget the semicolon at the end of the statement!											********
********																		********
******** If there is a match the function returns a table with 7 columns - Title, Author, Magazine, Year, Pages, ISBN and Keywords.			********
******** The function will return no data if there is no match.												********
******** Please note that the column Keywords is filled with a result that looks like {keyw1,keyw2,keyw3} i.e. the column contains all keywords.	********
****************************************************************************************************************************************************************
*/
create or replace function searchByKeyword(_Keyword text) 
RETURNS table(Title character varying,Author character varying,Magazine character varying, Year int,Pages int,ISBN character varying,Keywords text[]) 
LANGUAGE SQL as $body$
select * from "Books" where "ISBN" in (
select "ISBN" 
from (select "Title", "Author", "Magazine", "Year Published", "Pages", "ISBN", unnest("Keywords") keyword from "Books" ) x
where Keyword ilike _Keyword);
$body$;

/* 
****************************************************************************************************************************************************************
********								Search By Author v.1.0								********
******** This function should be called when a search is performed by Author.										********
******** Function usage:																********
********	select * from searchByAuthor('%author_name%');												********
******** where author_name is the name passed by the user.												********
********																		********
******** It is recomended to use percent signs around the name when creating the query as it's possible that the user has entered only partial text.	********
******** Example code:																	********
********	qry = "SELECT * from searchByAuthor('%" + myString + "%');" ;										********
********																		********
******** Don't forget the semicolon at the end of the statement!											********
********																		********
******** If there is a match the function returns a table with 7 columns - Title, Author, Magazine, Year, Pages, ISBN and Keywords.			********
******** The function will return no data if there is no match.												********
******** Please note that the column Keywords is filled with a result that looks like {keyw1,keyw2,keyw3} i.e. the column contains all keywords.	********
****************************************************************************************************************************************************************
*/
create or replace function searchByAuthor(_Author text)
RETURNS table(Title character varying,Author character varying,Magazine character varying, Year int,Pages int,ISBN character varying,Keywords text[]) 
LANGUAGE SQL as $body$
select * from "Books" where "Author" ilike _Author;
$body$;



/* 
****************************************************************************************************************************************************************
********								Search By Title v.1.0								********
******** This function should be called when a search is performed by Title.										********
******** Function usage:																********
********	select * from searchByTitle('%Title%');													********
******** where Title is the name passed by the user.													********
********																		********
******** It is recomended to use percent signs around the title when creating the query as it's possible that the user has entered only partial text.	********
******** Example code:																	********
********	qry = "SELECT * from searchByTitle('%" + myString + "%');" ;										********
********																		********
******** Don't forget the semicolon at the end of the statement!											********
********																		********
******** If there is a match the function returns a table with 7 columns - Title, Author, Magazine, Year, Pages, ISBN and Keywords.			********
******** The function will return no data if there is no match.												********
******** Please note that the column Keywords is filled with a result that looks like {keyw1,keyw2,keyw3} i.e. the column contains all keywords.	********
****************************************************************************************************************************************************************
*/

create or replace function searchByTitle(_Title text)
RETURNS table(Title character varying,Author character varying,Magazine character varying, Year int,Pages int,ISBN character varying,Keywords text[]) 
LANGUAGE SQL as $body$
select * from "Books" where "Title" ilike _Title;
$body$;


/* 
****************************************************************************************************************************************************************
********								Select all entries v.1.0							********
******** This view should be called when a search is performed whitout any additional data(no keyword,author or title).					********
******** View usage:																	********
********	SELECT * from selectBook;														********
********																		********
******** Example code:																	********
********	qry = "SELECT * from selectBook;" ;													********
********																		********
******** Don't forget the semicolon at the end of the statement!											********
********																		********
******** If there is any content available the function returns a table with 7 columns - Title, Author, Magazine, Year, Pages, ISBN and Keywords.	********
******** The function will return no data if there is no match.												********
******** Please note that the column Keywords is filled with a result that looks like {keyw1,keyw2,keyw3} i.e. the column contains all keywords.	********
****************************************************************************************************************************************************************
*/

create or replace view selectBook as select * from "Books";

/* 								END OF SEARCH FUNCTIONS	SECTION									 */








/* 								START OF INSERT FUNCTIONS SECTION								 */


/* 
****************************************************************************************************************************************************************
********								Insert data v.0.6								********
******** 					!!!Please note that this function still requires additional updates!!!					********
******** To be added:																	********
******** 	1) Function should return 1 on successfull insert and 0 when the data already exists (checking by ISBN as unique).			********
********																		********
******** This function should be called when a new data is to be inserted in the database.								********
******** Function usage:																********
********	SELECT * from fillBook('Title','Author','Magazine',Year_INT,Pages_INT,'ISBN','{keyword1,keyword2,keywordX}');				********
******** Please note the way keywords are entered! The expected format is '{keyword,keyword}'. If no keywords are provided use '{}'. Each keyword is 	********
******** separated with comma (,) from the next one.													********
********																		********
******** Example code:																	********
********	qry = "SELECT * from fillBook('Title','Author','Magazine',1777,70,'ISBN19','{keyword1,keyword2,keywordX}');";				********
********																		********
******** Don't forget the semicolon at the end of the statement!											********
********																		********
******** At the moment this function returns void. Further revision will update the function to return 1 or 0 depending if the data already exists.	********
****************************************************************************************************************************************************************
*/

CREATE or REPLACE FUNCTION fillBook(_Title text, _Author text, _Magazine text, _Year int, _Pages int, _ISBN text, _Keywords text[]) RETURNS void AS $$
    BEGIN
	INSERT INTO "Books" VALUES (_Title, _Author, _Magazine, _Year, _Pages, _ISBN, _Keywords);
    END;
$$ LANGUAGE plpgsql;




/* 								END OF INSERT FUNCTIONS SECTION								 	 */



/* 								START OF DELETE FUNCTIONS SECTION								 */


/* 
****************************************************************************************************************************************************************
********								Delete data row v.0.4								********
******** 					!!!Please note that this function still requires additional updates!!!					********
******** To be added:																	********
******** 	1) Function should return 1 on successfull delete and 0 when the delete has failed.							********
********																		********
******** This function should be called when an existing data is to be deleted from the database. Function uses the ISBN to uniquely identify the data.	********
******** Function usage:																********
********	SELECT * from dellBook('ISBN');														********
******** Where ISBN is the actual ISBN of the data that is to be deleted.									 	********
********																		********
******** Example code:																	********
********	qry = "SELECT * dellBook('ISBN');";													********
********																		********
******** Don't forget the semicolon at the end of the statement!											********
********																		********
******** At the moment this function returns void. Further revision will update the function to return 1 or 0 depending if the entry already exists.	********
****************************************************************************************************************************************************************
*/


create or replace function dellBook(_ISBN text) returns void as $$
	begin
		delete from "Books" where "ISBN" = _ISBN;
	end;
$$ language plpgsql;


/* 								END OF DELETE FUNCTIONS SECTION								 	 */