// Formatted last-modified date
if (Date.parse(document.lastModified) != 0)
{
  var modiDate = new Date(document.lastModified);
  var modiYear = modiDate.getYear();

  modiYear = (modiYear < 2000) ? modiYear + 1900 : modiYear;

  var monthName = new Array("January", "February", "March", "April",
    "May", "June", "July", "August", "September", "October", "November",
    "December");

  document.write(modiDate.getDate() + " " +
    monthName[modiDate.getMonth()] + " " + modiYear);
}
//
